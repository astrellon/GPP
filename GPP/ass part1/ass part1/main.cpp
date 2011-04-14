// CSE4GPP Assignment part 1
// Alan Lawrey 15547299

// Main file that defines the render function, physics function, initialization
// of spheres and OpenGL.
//
// All code related to WinMain is in NeHeGL.h.

#include "main.h"

// Navigation related
int oldMouseButton = 0, oldX = -1, oldY = -1;
bool mouseLeftDown = false;
bool mouseRightDown = false;
float zoom = 90;
float camRotateX = 0.0f, camRotateY = 0.0f;


// FPS and PPS related
int *physicsFrames, renderFrame = 0;
int renderFrameCounter = 0;
int framesPerSecond = 0;
float physicsPerSecond = 0;

double freq;

LARGE_INTEGER *updateTimes, frequency;


// Threading related
int numPhysicsThreads = 0;
boost::mutex runningLock;
boost::thread_group threads;

int rendering = 0;
int runningThreads = 0;
int pauseSimulation = 0;
// Should an error occur during initialization, such as SSE not being
// supported during execution of an SSE build. This will be set to true
// and the program will only display an error message.
bool displayError = false;

// Physics related
int numSpheres = 0;
Vector3 *spherePositions;
SphereData *sphereData;
void physicsThread(int threadNum);

// Rendering related
int screenWidth = 800, screenHeight = 600;
TextField txtNumThreads, txtInstructions, txtInstructions2, txtNumSpheres, txtRenderFrames, txtPhysicsFrames;
Mesh sphereMesh;

// Shader support
GLuint g_program;
GLuint g_programCameraPositionLocation;
GLuint g_programObjectPositionLocation;
GLuint g_programLightDirectionLocation;
GLuint g_programUserSphereLocation;

float g_cameraPosition[3];
float g_lightDirection[3];

// Sets the size of the user controlled sphere. This can be changed
// by usings the number keys.
float userSphereSize = 1.0f;

// Used as the F(t + dt, p(t + dt), v(t + dt)) equation.
FORCEINLINE void calcForce(Vector3 &forces, const float &springConst, const float &dampFactor, const float &overlap, const Vector3 &tangent, const Vector3 &newVelocity)
{
	Vector3 vs = tangent * dot3(tangent, newVelocity);
	forces -= (springConst * overlap * tangent) + (dampFactor * vs);
}

// These two functions are related to stopping and starting the physics and 
// rendering when we want to add/remove spheres. While this is slow it 
// should only happen on occasional user input.

// Called when a thread wants to indicate that it has stopped.
void threadStopped()
{
	runningLock.lock();
	runningThreads--;
	runningLock.unlock();
}

// Called when a thread wants to indicate that it has started.
void threadStarted()
{
	runningLock.lock();
	runningThreads++;
	runningLock.unlock();
}

// Adds or removes spheres, waits for all the physics and rendering threads
// to stop so the sphere arrays can be safely modified.
//
// num: A positive value will add spheres, a negative value will remove
//		'num' spheres from the end of the sphere arrays.
void changeSphereCount(int num)
{
	// Set that the simulation needs to be paused and waits for the threads
	// to stop.

#if _USE_MT
	pauseSimulation = 1;
	while(runningThreads > 0)
	{
		boost::this_thread::yield();
	}
#endif

	int newSize = numSpheres + num;
	if(newSize <= 0)
	{
		newSize = 0;
	}
	else
	{
		// First time we have to allocate the memory, however after that we can
		// reallocate, which if memory is available at the end of the current arrays
		// should take less time.
		if(sphereData == NULL)
		{
			spherePositions = (Vector3 *)_aligned_malloc(sizeof(Vector3) * newSize, 16);
			sphereData = (SphereData *)_aligned_malloc(sizeof(SphereData) * newSize, 16);
		}
		else
		{
			spherePositions = (Vector3 *)_aligned_realloc(spherePositions, sizeof(Vector3) * newSize, 16);
			sphereData = (SphereData *)_aligned_realloc(sphereData, sizeof(SphereData) * newSize, 16);
		}
	}

	// If we have added spheres, we need to initialize them.
	if(newSize > numSpheres)
	{
		int x = -4;
		int z = -4;
		int y = 0;

		float space = (float)ROOM_SIZE / 9.0f;
		
		for(int i = numSpheres; i < newSize; i++)
		{
			int mod = i % 3;
			float radius = (float)(mod + 1) * 0.5f;
			float roomSize = ROOM_SIZE - radius;
			// Check if we need to initialize the user sphere.
			if(i == 0)
			{
				spherePositions[0] = Vector3(0, -ROOM_SIZE + userSphereSize, 0);
			}
			else
			{
				//spherePositions[i] = Vector3(randf() * roomSize * 2.0f - roomSize, randf() * newSize * 0.02f - roomSize + 20.0f, randf() * roomSize * 2.0f - roomSize);
				spherePositions[i] = Vector3(x * space, (mod * -space + y * space * 3.0f) * 0.75f, z * space);
				spherePositions[i] += Vector3(randf() - 0.5f, randf() - 0.5f, randf() - 0.5f);
				if(mod == 2)
				{
					x++;
					if(x > 4)
					{
						x = -4;
						z++;
						if(z > 4)
						{
							z = -4;
							y++;
						}
					}
				}
			}
			SphereData &sphere = sphereData[i];
			sphere.m_velocity = Vector3(0);
			sphere.m_acc = Vector3(0);
		}
	}

	numSpheres = newSize;

	// Update the spheres indicator.
	char buff[32];
	sprintf(buff, "Num Spheres: %d", numSpheres);
	txtNumSpheres.setText(buff);

#if _USE_MT
	// Turn pausing off and the physics and rendering threads will start up
	// again next time they switch on.
	pauseSimulation = 0;
#endif
}

// Initializes OpenGL. Loads the vertex and fragment shaders, and creates the
// sphere mesh.
void glInit(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	// Enable texture for the text fields.
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

		// get the program info log
		glGetProgramiv(g_program, GL_INFO_LOG_LENGTH, &length);
		log = (char *)malloc(length);
		glGetProgramInfoLog(g_program, length, &result, log);

		// print an error message and the info log
		fprintf(stderr, "init(): Program linking failed: %s\n", log);
		free(log);

		// delete the program
		glDeleteProgram(g_program);
		g_program = 0;
	}

	// get uniform locations
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
	// Initialize the DevIL framework.
	ilInit();

	// Initialize OpenGL
	glInit();

	// Give the scene a starting number of spheres.
	changeSphereCount(INITIAL_SPHERE_COUNT);
	
	// Create an image ID for our font texture.
	ILubyte fontId = ilGenImage();
	ilBindImage(fontId);

	if(!ilLoad(IL_PNG, "data/font.png"))
	{
		MessageBox(NULL, "Unable to load font texture.\nThere will be no interface.", "Could not load font.", MB_OK);
	}
	else
	{
		// Tell all existing TextFields the image ID to use.
		txtNumSpheres.setFontId(fontId);
		txtRenderFrames.setFontId(fontId);
		txtPhysicsFrames.setFontId(fontId);
		txtNumThreads.setFontId(fontId);

		txtInstructions.setFontId(fontId);
		txtInstructions.setColour(0xFFFFAA);
		txtInstructions.setText("Press +/- to add or remove spheres. 1 - 9 to set sphere size.");

		txtInstructions2.setFontId(fontId);
		txtInstructions2.setColour(0xFFFFAA);
		txtInstructions2.setText("Use the arrow keys and page up/down to control the sphere.");
	}

	int iCPUVals[4];
	cpuid(1, iCPUVals);

	bool sse4Support = false;
	if(iCPUVals[2] & (1 << 19))
	{
		sse4Support = true;
	}

#ifdef _SSE4
	if(!sse4Support && _SSE4)
	{
		txtInstructions.setColour(0xFF0000);
		txtInstructions.setText("Your computer does not support SSE4, please use a different version of this program.");
		displayError = true;

		numPhysicsThreads = 1;
	}
	else
#endif
	{
		// Setup performance checking, used to determine time difference
		// between physics thread runs.
		QueryPerformanceFrequency( &frequency );
		freq = (double)frequency.QuadPart / 1000.0;
		LARGE_INTEGER firstTime;
		QueryPerformanceCounter(&firstTime);

	#if _USE_MT
		// Get the number of physics threads to use from boost.
		numPhysicsThreads = boost::thread::hardware_concurrency();
	#else
		// Set that we're only using one thread, mostly for the purpose that
		// the physics function uses the number of threads for striding.
		numPhysicsThreads = 1;
	#endif

		updateTimes = new LARGE_INTEGER[numPhysicsThreads];
		physicsFrames = new int[numPhysicsThreads];

		for(int i = 0; i < numPhysicsThreads; i++)
		{
			updateTimes[i] = firstTime;
			physicsFrames[i] = 0;
	#if _USE_MT
			threads.create_thread(boost::bind(physicsThread, i));
	#endif
		}

		char buff[32];

		sprintf(buff, "Num threads: %d", numPhysicsThreads);
		txtNumThreads.setText(buff);
	}
}

// Setup the projection matrix for perspective.
// Used to render the scene.
void perspectiveView()
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	GLfloat zNear = 0.1f;
	GLfloat zFar = 1000.0f;
	GLfloat aspect = float(screenWidth)/float(screenHeight);
	GLfloat fH = tan( float(65.0f / 360.0f * 3.14159f) ) * zNear;
	GLfloat fW = fH * aspect;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );

	glMatrixMode(GL_MODELVIEW);
}

// Setup the projection matrix for orthographic.
// Used to render the interface.
void orthographicView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, screenWidth, screenHeight, 0, 0, 1);
	
	glMatrixMode(GL_MODELVIEW);
}

// Called before every render.
void update(DWORD milli)
{
	if(displayError)
		return;

	static DWORD time = 0;
	if(numSpheres > 0)
	{
		// Manually set the velocity of the user sphere.
		sphereData[0].m_velocity = Vector3(0);
		
		// left arrow
		if(keysDown[37] || keysDown['A'])
			sphereData[0].m_velocity += Vector3(-USER_SPHERE_SPEED, 0.0, 0.0);
		// right arrow
		if(keysDown[39] || keysDown['D'])
			sphereData[0].m_velocity += Vector3(USER_SPHERE_SPEED, 0.0, 0.0);

		// up arrow
		if(keysDown[38] || keysDown['W'])
			sphereData[0].m_velocity += Vector3(0.0f, 0.0, -USER_SPHERE_SPEED);
		// down arrow
		if(keysDown[40] || keysDown['S'])
			sphereData[0].m_velocity += Vector3(0.0f, 0.0, USER_SPHERE_SPEED);

		// page up
		if(keysDown[33] || keysDown['Q'])
			sphereData[0].m_velocity += Vector3(0.0f, USER_SPHERE_SPEED, 0);
		// page down
		if(keysDown[34] || keysDown['E'])
			sphereData[0].m_velocity += Vector3(0.0f, -USER_SPHERE_SPEED, 0.0);
	}

	// Rough second counter by adding up all the previous dt times.
	time += milli;

	if(time > 1000)
	{
		framesPerSecond = renderFrameCounter;

		// Average the physics frame counter.
		physicsPerSecond = 0.0f;

		for(int i = 0; i < numPhysicsThreads; i++)
		{
			physicsPerSecond += physicsFrames[i];
			physicsFrames[i] = 0;
		}

		renderFrameCounter = 0;
		physicsPerSecond /= numPhysicsThreads;
		
		time -= 1000;
	}

#if _USE_MT != 1
	// If we are not using multi threads. Then we need to call the physics
	// function here, after we're done the user input and before the scene
	// is rendered.
	physicsThread(0);
#endif
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if(displayError)
	{
		orthographicView();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		txtInstructions.render((screenWidth - txtInstructions.getTextureWidth()) / 2.0f, (screenHeight - txtInstructions.getTextureHeight()) / 2.0f);

		glFlush();
		return;
	}

	glPushMatrix();

	// Setup the navigation
	glTranslated(0, 0, -zoom);
	glRotatef(camRotateX, 1, 0, 0);
	glRotatef(camRotateY, 0, 1, 0);
	glTranslated(0, ROOM_SIZE, 0);

	// Setup drawing for the scene.
	perspectiveView();
	
#if _USE_MT
	// If the render thread needs to wait for new spheres to be added.
	while(pauseSimulation)
	{
		boost::this_thread::yield();
	}

	threadStarted();
#endif

	renderFrame++;
	renderFrameCounter++;

	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 1.0f, 1.0f);
	
	glUseProgram(g_program);
	glUniform3fv(g_programCameraPositionLocation, 1, g_cameraPosition);
	glUniform3fv(g_programLightDirectionLocation, 1, g_lightDirection);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereMesh.indexBufferId);

	// We are using instanced rendering, however we cannot draw all the spheres at once.
	// So we have a maximum number of objects that can be rendered at once and we'll just
	// render that many at a time until we have less spheres than MAX_OBJECTS_PER_RENDER 
	// left to render.
	for(int objectIndex = 0; objectIndex < numSpheres; objectIndex += MAX_OBJECTS_PER_RENDER)
	{
		int numObjects = MAX_OBJECTS_PER_RENDER;
		// Are we going to be rendering more spheres than we should?
		if (numObjects + objectIndex > numSpheres)
		{
			numObjects = numSpheres - objectIndex;
		}
		Vector3 *positionIndex = spherePositions + objectIndex;
		glUniform4fv(g_programObjectPositionLocation, numObjects, (float *)positionIndex);
		// Tell the shader the size of the user sphere.
		// Due to the way the shader renders multiple instances, it only knows the id
		// of the current instance it is rendering, which does not reflect the sphere's actual
		// id.
		// So we will set that the size of the userSphere only on the first instanced render
		// and all later renders it will be zero.
		glUniform1f(g_programUserSphereLocation, (objectIndex == 0) * userSphereSize);

		glDrawElementsInstancedEXT(GL_TRIANGLES, sphereMesh.numIndicies, GL_UNSIGNED_INT, 0, numObjects);
	}

	glUseProgram(0);
	
#if _USE_MT
	threadStopped();
#endif

	glPopMatrix();

	// Setup drawing for the interface.
	orthographicView();

	char buff[64];

	sprintf(buff, "FPS: %d", framesPerSecond);
	txtRenderFrames.setText(buff);

	sprintf(buff, "PPS: %.1f", physicsPerSecond);
	txtPhysicsFrames.setText(buff);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float yPos = -2.0f;
	
	txtInstructions.render(10, yPos += 12.0f);
	txtInstructions2.render(10, yPos += 12.0f);
	txtNumThreads.render(10, yPos += 12.0f);
	txtNumSpheres.render(10, yPos += 12.0f);
	txtRenderFrames.render(10, yPos += 12.0f);
	txtPhysicsFrames.render(10, yPos += 12.0f);
	
	glDisable(GL_BLEND);

	glFlush();
}

void reshape (int w, int h)
{
	// Screen size is used by both the perspective and orthographic view functions.
	screenWidth = w;
	screenHeight = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void mouseFunc(int mouseButton, int x, int y)
{
	// If we are pressing a mouse button that was different to the last
	// time we had a mouse move event, then we want to reset the oldX and oldY
	// coordinates.
	if(mouseButton != oldMouseButton)
	{
		oldX = x;
		oldY = y;
	}
	
	// If the user is pressing the left mouse button.
	if(mouseButton == 1)
	{
		int dx = x - oldX;
		int dy = y - oldY;

		camRotateX += (float)dy * 0.5f;
		camRotateY += (float)dx * 0.5f;
	}
	// If the user is pressing the right mouse button.
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
			changeSphereCount(1000);
		}
		// - key
		else if(key == 109)
		{
			changeSphereCount(-1000);
		}

		// Change the diameter of the user sphere to be 
		// one more than the number of the key pressed.
		if(key >= '1' && key <= '9')
		{
			userSphereSize = (float)(key - '1' + 1);
		}
	}
}

// Physics thread that controls the motion of all the spheres.
void physicsThread(int threadNum)
{
	int localFrame = 0;

	// Define the normals for each of the walls.
	static const Vector3 bottom(0, 1, 0);
	static const Vector3 left(1, 0, 0);
	static const Vector3 right(-1, 0, 0);
	static const Vector3 front(0, 0, -1);
	static const Vector3 back(0, 0, 1);

	// Initialize the starting values for the wall and sphere, spring and damping values.
	float wallSpringConst = UPPER_WALL_SPRING_CONST;
	float sphereSpringConst = UPPER_SPHERE_SPRING_CONST;

	float wallDampFactor = UPPER_WALL_DAMP_FACTOR;
	float sphereDampFactor = UPPER_SPHERE_DAMP_FACTOR;

	// The physics thread itself deals with reseting its frame counter. 
	// It will do this once a second.
	float secondCheck = 0.0f;

#if _USE_MT
	// If the physics function is being called as a thread, then we don't want it 
	// to finish after one pass.
	int threadRunning = 0;
	while(programRunning)
	{
		// If the simulation needs to be paused but the thread is running,
		// stop.
		if(pauseSimulation && threadRunning)
		{
			threadRunning = 0;
			threadStopped();
		}
		// If the simulation needs to be running but the thread is stopped,
		// start.
		else if(!pauseSimulation && !threadRunning)
		{
			threadRunning = 1;
			threadStarted();
		}

		// If the thread isn't running at this time we can't go any further and 
		// we'll just wait and check if the thread has started up next time the
		// thread is executing.
		if(!threadRunning)
		{
			boost::this_thread::yield();
			continue;
		}
#endif

		localFrame++;

		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		// Get the time since this thread last executed and convert into
		// seconds (from milliseconds).
		float dt = (float)(((double)time.QuadPart - (double)updateTimes[threadNum].QuadPart) / freq);
		dt *= 0.001;

		// Adjust the spring and dampening values based on dt.
		// This helps when going to lower frame rates and the overlap between
		// the walls and spheres when using high frame rate values will cause
		// the spheres to gain energy.
		wallSpringConst = WALL_SPRING_GRAD * dt + WALL_SPRING_CONST;
		sphereSpringConst = SPHERE_SPRING_GRAD * dt + SPHERE_SPRING_CONST;

		wallDampFactor = WALL_DAMP_GRAD * dt + WALL_DAMP_CONST;
		sphereDampFactor = SPHERE_DAMP_GRAD * dt + SPHERE_DAMP_CONST;

		// As the gradients for the spring and dampening factors are negative,
		// we want to clamp the lower bounds of the values. Otherwise at low
		// framerates the values will be negative.
		if (wallSpringConst < LOWER_WALL_SPRING_CONST)
			wallSpringConst = LOWER_WALL_SPRING_CONST;

		if (sphereSpringConst < LOWER_SPHERE_SPRING_CONST)
			sphereSpringConst = LOWER_SPHERE_SPRING_CONST;

		if (wallDampFactor < LOWER_WALL_DAMP_FACTOR)
			wallDampFactor = LOWER_WALL_DAMP_FACTOR;

		if (sphereDampFactor < LOWER_SPHERE_DAMP_FACTOR)
			sphereDampFactor = LOWER_SPHERE_DAMP_FACTOR;

		// If this is the 1st thread, then we will deal with the user controlled
		// sphere here.
		int startSphere = threadNum;
		if(threadNum == 0)
		{
			// Skip calcuating the user sphere like a typical sphere.
			startSphere += numPhysicsThreads;

			// As the user controlled sphere only has a position and velocity
			// that is either zero or constant, we can easily calculate its new
			// position.
			if(numSpheres > 0)
			{
				spherePositions[0] += sphereData[0].m_velocity * dt;
			}
		}

		for(int i = startSphere; i < numSpheres; i += numPhysicsThreads)
		{
			// Calculate the radius of the sphere based on array index.
			float radius = (float)((i % 3) + 1) * 0.5f;
			float roomSize = ROOM_SIZE - radius;
			
			Vector3 forces(0);

			Vector3 &spherePosition = spherePositions[i];
			SphereData &sphere = sphereData[i];

			// Calculate the interim velocity.
			Vector3 halfVelo = sphere.m_velocity + (0.5f * sphere.m_acc * dt);
			spherePosition += halfVelo * dt;

			Vector3 tempVelocity = sphere.m_velocity + (sphere.m_acc * dt);

			float overlap;

			// As the % operator is fairly slow we can take advantage here
			// that we always know what the next value in the array is going
			// to be and manually determine the mod.
			int indexCounter = 0;
			
			for(int j = 0; j < numSpheres; j++)
			{
				// And since we don't want the actual mod value but mod + 1
				// we don't worry about resetting to 0.
				indexCounter++;
				if(indexCounter > 3)
					indexCounter = 1;

				// We don't want a sphere to check if it's collided with itself.
				if(i == j)
					continue;

				SphereData &otherSphere = sphereData[j];
				Vector3 toCentre = spherePosition - spherePositions[j];
				// An unfortunately slow way of checking if the radius of the
				// other sphere should be the other spheres actual radius or
				// the user controlled spheres radius.
				float otherRadius; 
				if(j == 0)
				{
					otherRadius = userSphereSize;
				}
				else
				{
					otherRadius = (float)(indexCounter) * 0.5f;
				}

				float combinedRadius = radius + otherRadius;
				float lengthSqrd = lengthSquared(toCentre);
				float combinedRadiusSqrd = combinedRadius * combinedRadius;
				
				// A check to see if the spheres have overlapped at all.
				float overlapSqrd = lengthSqrd - combinedRadiusSqrd;
				if(overlapSqrd < 0)
				{
#if _SSE
					overlap = sqrt(lengthSqrd) - combinedRadius;
					// We want to let the vector normalize itself in SSE
					// because we can take advantage of the rsqrt instruction
					// which will be quicker than loading in a float value
					// and multiplying by the reciprocal.
					Vector3 tangent = normalize(toCentre);
#else
					// Here we can take advantage that we've already calculated
					// the actual length value so that we have the overlap and
					// can use that value again to normalize the tangent.
					float len = sqrt(lengthSqrd);
					overlap = len - combinedRadius;
					Vector3 tangent = toCentre / len;
#endif
					calcForce(forces, sphereSpringConst, sphereDampFactor, overlap, tangent, tempVelocity);
				}
			}
			
			// Calculate the overlap with the walls using the normal of the wall
			// and the walls distance from the origin. Should work best for SSE
			// as it should not require any checking of individual elements of
			// the vector.
			overlap = dot3(spherePosition, bottom) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, wallSpringConst, wallDampFactor, overlap, bottom, tempVelocity);
			}

			overlap = dot3(spherePosition, left) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, wallSpringConst, wallDampFactor, overlap, left, tempVelocity);
			}
			overlap = dot3(spherePosition, right) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, wallSpringConst, wallDampFactor, overlap, right, tempVelocity);
			}

			overlap = dot3(spherePosition, front) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, wallSpringConst, wallDampFactor, overlap, front, tempVelocity);
			}
			overlap = dot3(spherePosition, back) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, wallSpringConst, wallDampFactor, overlap, back, tempVelocity);
			}

			// Apply the accumulated forces to the acceleration.
			sphere.m_acc = forces / (radius * 2.0f);
			sphere.m_acc += GRAVITY;

			// Calculate the final velocity value from the interim velocity
			// and final acceleration.
			sphere.m_velocity = halfVelo + (0.5f * sphere.m_acc * dt);
		}

		// Determin if we need to reset out physics frame counter.
		secondCheck += dt;
		if(secondCheck > 1.0f)
		{
			physicsFrames[threadNum] = 1;
			secondCheck -= 1.0f;
		}
		else
		{
			physicsFrames[threadNum]++;
		}

		updateTimes[threadNum] = time;

		// If we're running the physics function as a thread function then we
		// need to keep it running, so this is the end of the while(programRunning) loop.
#if _USE_MT
	}
#endif
}

// Sets that the program should finish.
void deinit()
{
	programRunning = FALSE;
#if _USE_MT
	threads.join_all();
#endif

	_aligned_free(spherePositions);
	_aligned_free(sphereData);
}

void terminateApplication (GL_Window* window)							// Terminate The Application
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	programRunning = FALSE;											// Stop Looping Of The Program
	
}