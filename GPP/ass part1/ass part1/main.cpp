#include "main.h"

int numSpheres = 0;

int oldMouseButton = 0, oldX = -1, oldY = -1;
int renderFrame = 0;
int physicsFrame = 0;

int renderFrameCounter = 0;
int physicsFrameCounter = 0;

int framesPerSecond = 0;
int physicsPerSecond = 0;

LARGE_INTEGER frequency, timeStart, timeEnd;
double freq, totalTime;

bool mouseLeftDown = false;
bool mouseRightDown = false;

float zoom = 90;
float camRotateX = 90.0f, camRotateY;

int performanceCount = 0;
int numPhysicsThreads = 0;
LARGE_INTEGER *updateTimes;

boost::mutex runningLock;
boost::mutex physicsLock;
boost::thread_group threads;

int rendering = 0;
int runningThreads = 0;
int pauseSimulation = 0;
int physicsRunning = 0;

TextField txtNumThreads, txtInstructions, txtNumSpheres, txtRenderFrames, txtPhysicsFrames;

int screenWidth = 800, screenHeight = 600;

Vector3 *spherePositions;

typedef struct
{
	Vector3 m_velocity;
	Vector3 m_acc;
} Sphere;

Sphere *sphereData;

Mesh sphereMesh;

// RENDERING VARIABLES

// Shader support
GLuint g_program;
GLuint g_programCameraPositionLocation;
GLuint g_programObjectPositionLocation;
GLuint g_programLightDirectionLocation;
GLuint g_programUserSphereLocation;

float g_cameraPosition[3];
float g_lightDirection[3];

float userSphereSize = 10.0f;

// END RENDERING VARIABLES

void physicsThread(int threadNum);

FORCEINLINE void calcForce(Vector3 &forces, const float &springConst, const float &dampFactor, const float &overlap, const Vector3 &tangent, const Vector3 &newVelocity)
{
	Vector3 vs = tangent * dot3(tangent, newVelocity);
	forces -= (springConst * overlap * tangent) + (dampFactor * vs);
}

void physicsFrameStart()
{
	physicsLock.lock();
	physicsRunning++;
	physicsLock.unlock();
}

void physicsFrameFinish()
{
	physicsLock.lock();
	physicsRunning--;
	if(physicsRunning == 0)
	{
		physicsFrame++;
		physicsFrameCounter++;
	}
	physicsLock.unlock();
}

void threadStopped()
{
	runningLock.lock();
	runningThreads--;
	runningLock.unlock();
}

void threadStarted()
{
	runningLock.lock();
	runningThreads++;
	runningLock.unlock();
}

void addSpheres(int num)
{
	pauseSimulation = 1;
	while(runningThreads > 0)
	{
		boost::this_thread::yield();
	}
	int newSize = numSpheres + num;
	if(newSize <= 0)
	{
		newSize = 0;
	}
	else
	{
		if(sphereData == NULL)
		{
			spherePositions = (Vector3 *)_aligned_malloc(sizeof(Vector3) * newSize, 16);
			sphereData = (Sphere *)_aligned_malloc(sizeof(Sphere) * newSize, 16);
		}
		else
		{
			spherePositions = (Vector3 *)_aligned_realloc(spherePositions, sizeof(Vector3) * newSize, 16);
			sphereData = (Sphere *)_aligned_realloc(sphereData, sizeof(Sphere) * newSize, 16);
		}
	}

	int len = newSize > numSpheres ? numSpheres : newSize;

	if(newSize > numSpheres)
	{
		for(int i = numSpheres; i < newSize; i++)
		{
			float radius = (float)(i % 3) * 0.5f + 0.5f;
			float roomSize = ROOM_SIZE - radius;
			if(i == 0)
			{
				spherePositions[0] = Vector3(0, -ROOM_SIZE + userSphereSize, 0);
			}
			else
			{
				spherePositions[i] = Vector3(randf() * roomSize * 2.0f - roomSize, randf() * newSize * 0.02f - roomSize + 20.0f, randf() * roomSize * 2.0f - roomSize);
			}
			Sphere &sphere = sphereData[i];
			sphere.m_velocity = Vector3(0);
			sphere.m_acc = Vector3(0);
		}
	}

	numSpheres = newSize;

	char buff[32];
	sprintf(buff, "Num Spheres:    %d", numSpheres);
	txtNumSpheres.setText(buff);

	pauseSimulation = 0;
}

void glInit(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	g_program = glCreateProgram();
	shaderAttachFromFile(g_program, GL_VERTEX_SHADER, "data/vertex.vp");
	shaderAttachFromFile(g_program, GL_FRAGMENT_SHADER, "data/fragment.fp");

	GLint result;
	glLinkProgram(g_program);
	glGetProgramiv(g_program, GL_LINK_STATUS, &result);
	if(result == GL_FALSE) {
		GLint length;
		char *log;

		/* get the program info log */
		glGetProgramiv(g_program, GL_INFO_LOG_LENGTH, &length);
		log = (char *)malloc(length);
		glGetProgramInfoLog(g_program, length, &result, log);

		/* print an error message and the info log */
		fprintf(stderr, "init(): Program linking failed: %s\n", log);
		free(log);

		/* delete the program */
		glDeleteProgram(g_program);
		g_program = 0;
	}

	/* get uniform locations */
	g_programCameraPositionLocation = glGetUniformLocation(g_program, "cameraPosition");
	g_programObjectPositionLocation = glGetUniformLocation(g_program, "objectPositions");
	g_programLightDirectionLocation = glGetUniformLocation(g_program, "lightDirection");
	g_programUserSphereLocation = glGetUniformLocation(g_program, "userSphere");
	
	g_lightDirection[0] = 1.0f;
	g_lightDirection[1] = 0.0f;
	g_lightDirection[2] = 0.0f;

	g_cameraPosition[0] = 0.0f;
	g_cameraPosition[1] = 0.0f;
	g_cameraPosition[2] = -zoom;

	sphereMesh = createSphere(12, 12);
}

void init()
{
	ilInit();

	glInit();

	QueryPerformanceFrequency( &frequency );
	freq = (double)frequency.QuadPart / 1000.0, totalTime = 0.0;

	addSpheres(STARTING_BALLS);

	ILubyte fontId = ilGenImage();
	ilBindImage(fontId);

	if(!ilLoad(IL_PNG, "data/font.png"))
	{
		cout << "Unable to load font image" << endl;
	}
	else
	{
		txtNumSpheres.setFontId(fontId);
		txtRenderFrames.setFontId(fontId);
		txtPhysicsFrames.setFontId(fontId);
		txtNumThreads.setFontId(fontId);

		txtInstructions.setFontId(fontId);
		txtInstructions.setColour(0xFFFFAA);
		txtInstructions.setText("Press +/- to add or remove spheres. 1 - 9 to set sphere size.");
	}

	LARGE_INTEGER firstTime;
	QueryPerformanceCounter(&firstTime);

	numPhysicsThreads = boost::thread::hardware_concurrency();
	//numPhysicsThreads = 1;

	char buff[32];

	sprintf(buff, "Num threads:    %d", numPhysicsThreads);
	txtNumThreads.setText(buff);

	updateTimes = new LARGE_INTEGER[numPhysicsThreads];

	for(int i = 0; i < numPhysicsThreads; i++)
	{
		updateTimes[i] = firstTime;
		threads.create_thread(boost::bind(physicsThread, i));
	}
}

void perspectiveView()
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	GLfloat zNear = 0.1f;
	GLfloat zFar = 255.0f;
	GLfloat aspect = float(screenWidth)/float(screenHeight);
	GLfloat fH = tan( float(65.0f / 360.0f * 3.14159f) ) * zNear;
	GLfloat fW = fH * aspect;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );

	glMatrixMode(GL_MODELVIEW);
}

void orthographicView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, screenWidth, screenHeight, 0, 0, 1);
	
	glMatrixMode(GL_MODELVIEW);
}

void update(DWORD milli)
{
	static DWORD time = 0;
	if(numSpheres > 0)
	{
		const float speed = 20.0f;

		sphereData[0].m_velocity = Vector3(0);
		
		// left arrow
		if(keysDown[37] || keysDown['A'])
			sphereData[0].m_velocity += Vector3(-speed, 0.0, 0.0);
		// right arrow
		if(keysDown[39] || keysDown['D'])
			sphereData[0].m_velocity += Vector3(speed, 0.0, 0.0);

		// up arrow
		if(keysDown[38] || keysDown['W'])
			sphereData[0].m_velocity += Vector3(0.0f, 0.0, -speed);
		// down arrow
		if(keysDown[40] || keysDown['S'])
			sphereData[0].m_velocity += Vector3(0.0f, 0.0, speed);

		// page up
		if(keysDown[33] || keysDown['Q'])
			sphereData[0].m_velocity += Vector3(0.0f, speed, 0);
		// page down
		if(keysDown[34] || keysDown['E'])
			sphereData[0].m_velocity += Vector3(0.0f, -speed, 0.0);
	}

	time += milli;

	if(time > 1000)
	{
		framesPerSecond = renderFrameCounter;
		physicsPerSecond = physicsFrameCounter;
		renderFrameCounter = 0;
		physicsFrameCounter = 0;
		time = 0;
	}
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();

	glEnable(GL_LIGHTING);
	
	glTranslated(0, 0, -zoom);
	glRotatef(camRotateX, 1, 0, 0);
	glRotatef(camRotateY, 0, 1, 0);
	glTranslated(0, ROOM_SIZE, 0);

	perspectiveView();
	
	while(pauseSimulation)
	{
		boost::this_thread::yield();
	}

	renderFrame++;
	renderFrameCounter++;

	threadStarted();

	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 1.0f, 1.0f);
	
	glUseProgram(g_program);
	glUniform3fv(g_programCameraPositionLocation, 1, g_cameraPosition);
	glUniform3fv(g_programLightDirectionLocation, 1, g_lightDirection);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereMesh.indexBufferId);

	for(int objectIndex = 0; objectIndex < numSpheres; objectIndex += MAX_OBJECTS_PER_RENDER)
	{
		int numObjects = MAX_OBJECTS_PER_RENDER;
		if (numObjects + objectIndex > numSpheres)
		{
			numObjects = numSpheres - objectIndex;
		}
		Vector3 *positionIndex = spherePositions + objectIndex;
		glUniform4fv(g_programObjectPositionLocation, numObjects, (float *)positionIndex);

		glUniform1f(g_programUserSphereLocation, (objectIndex == 0) * userSphereSize);

		glDrawElementsInstancedEXT(GL_TRIANGLES, sphereMesh.numIndicies, GL_UNSIGNED_INT, 0, numObjects);
	}

	glUseProgram(0);
	
	threadStopped();
	glPopMatrix();

	glDisable(GL_LIGHTING);

	orthographicView();

	char buff[64];

	sprintf(buff, "Render frames:  %d (%d)", renderFrame, framesPerSecond);
	txtRenderFrames.setText(buff);

	sprintf(buff, "Physics frames: %d (%d)", physicsFrame, physicsPerSecond);
	txtPhysicsFrames.setText(buff);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	txtInstructions.render(10, 10);
	txtNumThreads.render(10, 22);
	txtNumSpheres.render(10, 34);
	txtRenderFrames.render(10, 46);
	txtPhysicsFrames.render(10, 58);
	
	glDisable(GL_BLEND);

	glFlush();
}

void reshape (int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}
/*
void redrawLoop(int value)
{
	display();
	glutTimerFunc(UPDATE_INTERVAL, redrawLoop, 0);
}

*/

void mouseFunc(int mouseButton, int x, int y)
{
	if(mouseButton != oldMouseButton)
	{
		oldX = x;
		oldY = y;
	}
	
	if(mouseButton == 1)
	{
		int dx = x - oldX;
		int dy = y - oldY;

		camRotateX += (float)dy * 0.5f;
		camRotateY += (float)dx * 0.5f;
	}
	else if(mouseButton == 2)
	{
		zoom += y - oldY;
	}

	oldX = x;
	oldY = y;

	oldMouseButton = mouseButton;
}

void keyboardFunc(BOOL *keys, int key)
{
	if(!keys[key])
	{
		// + key
		if(key == 107)
		{
			addSpheres(1000);
		}
		// - key
		else if(key == 109)
		{
			addSpheres(-1000);
		}

		if(key >= '1' && key <= '9')
		{
			userSphereSize = (float)(key - '1' + 1);
		}
	}
}

void physicsThread(int threadNum)
{
	int threadRunning = 0;
	int localFrame = 0;

	static const Vector3 bottom(0, 1, 0);
	static const Vector3 left(1, 0, 0);
	static const Vector3 right(-1, 0, 0);
	static const Vector3 front(0, 0, -1);
	static const Vector3 back(0, 0, 1);

	float floorSpringConst = UPPER_FLOOR_SPRING_CONST;
	float sphereSpringConst = UPPER_SPHERE_SPRING_CONST;

	float floorDampFactor = UPPER_FLOOR_DAMP_FACTOR;
	float sphereDampFactor = UPPER_SPHERE_DAMP_FACTOR;

	while(programRunning)
	{
		if(pauseSimulation && threadRunning)
		{
			threadRunning = 0;
			threadStopped();
		}
		else if(!pauseSimulation && !threadRunning)
		{
			threadRunning = 1;
			threadStarted();
		}

		if(!threadRunning)
		{
			boost::this_thread::yield();
			continue;
		}

		if(physicsFrame < localFrame)
		{
			boost::this_thread::yield();
			continue;
		}

		localFrame++;

		physicsFrameStart();

		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		float dt = (float)(((double)time.QuadPart - (double)updateTimes[threadNum].QuadPart) / freq);
		dt *= 0.001;

		floorSpringConst = FLOOR_SPRING_GRAD * dt + FLOOR_SPRING_CONST;
		sphereSpringConst = SPHERE_SPRING_GRAD * dt + SPHERE_SPRING_CONST;

		floorDampFactor = FLOOR_DAMP_GRAD * dt + FLOOR_DAMP_CONST;
		sphereDampFactor = SPHERE_DAMP_GRAD * dt + SPHERE_DAMP_CONST;

		if (floorSpringConst < LOWER_FLOOR_SPRING_CONST)
			floorSpringConst = LOWER_FLOOR_SPRING_CONST;

		if (sphereSpringConst < LOWER_SPHERE_SPRING_CONST)
			sphereSpringConst = LOWER_SPHERE_SPRING_CONST;

		if (floorDampFactor < LOWER_FLOOR_DAMP_FACTOR)
			floorDampFactor = LOWER_FLOOR_DAMP_FACTOR;

		if (sphereDampFactor < LOWER_SPHERE_DAMP_FACTOR)
			sphereDampFactor = LOWER_SPHERE_DAMP_FACTOR;

		int startSphere = threadNum;
		if(threadNum == 0)
		{
			startSphere += numPhysicsThreads;

			if(numSpheres > 0)
			{
				spherePositions[0] += sphereData[0].m_velocity * dt;
			}
		}

		for(int i = startSphere; i < numSpheres; i += numPhysicsThreads)
		{
			float radius = (float)((i % 3) + 1) * 0.5f;
			float roomSize = ROOM_SIZE - radius;
			
			Vector3 forces(0);

			Vector3 &spherePosition = spherePositions[i];
			Sphere &sphere = sphereData[i];

			Vector3 halfVelo = sphere.m_velocity + (0.5f * sphere.m_acc * dt);
			spherePosition += halfVelo * dt;

			Vector3 tempVelocity = sphere.m_velocity + (sphere.m_acc * dt);

			float overlap;

			int counter2 = 0;

			for(int j = 0; j < numSpheres; j++)
			{
				counter2++;
				if(counter2 > 3)
					counter2 = 1;

				if(i == j)
					continue;

				Sphere &otherSphere = sphereData[j];
				Vector3 toCentre = spherePosition - spherePositions[j];
				float otherRadius; 
				if(j == 0)
				{
					otherRadius = userSphereSize;
				}
				else
				{
					otherRadius = (float)(counter2) * 0.5f;
				}

				//float otherRadius = ((float)(counter2) * 0.5f * j != 0) + ((j == 0) * USER_SPHERE_SIZE);
				float combinedRadius = radius + otherRadius;
				float lengthSqrd = lengthSquared(toCentre);
				float combinedRadiusSqrd = combinedRadius * combinedRadius;
				
				float overlapSqrd = lengthSqrd - combinedRadiusSqrd;
				if(overlapSqrd < 0)
				{
					overlap = sqrt(lengthSqrd) - combinedRadius;
					Vector3 tangent = normalize(toCentre);
					calcForce(forces, sphereSpringConst, sphereDampFactor, overlap, tangent, tempVelocity);
				}
			}
			
			overlap = dot3(spherePosition, bottom) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, bottom, tempVelocity);
			}

			overlap = dot3(spherePosition, left) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, left, tempVelocity);
			}
			overlap = dot3(spherePosition, right) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, right, tempVelocity);
			}

			overlap = dot3(spherePosition, front) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, front, tempVelocity);
			}
			overlap = dot3(spherePosition, back) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, back, tempVelocity);
			}

			sphere.m_acc = forces / (radius * 2.0f);
			sphere.m_acc += GRAVITY;

			sphere.m_velocity = halfVelo + (0.5f * sphere.m_acc * dt);
		}

		updateTimes[threadNum] = time;

		physicsFrameFinish();
	}
}

void Deinitialize()
{
	programRunning = FALSE;
	threads.join_all();
}

void TerminateApplication (GL_Window* window)							// Terminate The Application
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	programRunning = FALSE;											// Stop Looping Of The Program
	
}