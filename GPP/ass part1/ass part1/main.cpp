#include <Windows.h>

#include "glut.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "IL\il.h"
#include <stdlib.h>

#define BOOST_NO_EXCEPTIONS

#include <boost\thread\thread.hpp>
#include <time.h>
#include <iostream>

using namespace std;

#include "Globals.h"
#include "Sphere.h"
#include "TextField.h"

#define UPDATE_INTERVAL 20
#define STARTING_BALLS 4000

#define UPPER_SPHERE_SPRING_CONST 200.0f
#define LOWER_SPHERE_SPRING_CONST 0.1f

#define UPPER_FLOOR_SPRING_CONST 800.0f
#define LOWER_FLOOR_SPRING_CONST 2.0f

#define UPPER_SPHERE_DAMP_FACTOR 2.0f
#define LOWER_SPHERE_DAMP_FACTOR 0.05f

#define UPPER_FLOOR_DAMP_FACTOR 10.0f
#define LOWER_FLOOR_DAMP_FACTOR 0.1f

#define UPPER_FRAME_STEP 0.02f
#define LOWER_FRAME_STEP 0.2f

#define FLOOR_SPRING_GRAD (LOWER_FLOOR_SPRING_CONST - UPPER_FLOOR_SPRING_CONST) / (LOWER_FRAME_STEP - UPPER_FRAME_STEP)
#define FLOOR_SPRING_CONST FLOOR_SPRING_GRAD * UPPER_FRAME_STEP + UPPER_FLOOR_SPRING_CONST

#define SPHERE_SPRING_GRAD (LOWER_SPHERE_SPRING_CONST - UPPER_SPHERE_SPRING_CONST) / (LOWER_FRAME_STEP - UPPER_FRAME_STEP)
#define SPHERE_SPRING_CONST SPHERE_SPRING_GRAD * UPPER_FRAME_STEP + UPPER_SPHERE_SPRING_CONST

#define FLOOR_DAMP_GRAD (LOWER_FLOOR_DAMP_FACTOR - UPPER_FLOOR_DAMP_FACTOR) / (LOWER_FRAME_STEP - UPPER_FRAME_STEP)
#define FLOOR_DAMP_CONST FLOOR_DAMP_GRAD * UPPER_FRAME_STEP + UPPER_FLOOR_DAMP_FACTOR

#define SPHERE_DAMP_GRAD (LOWER_SPHERE_DAMP_FACTOR - UPPER_SPHERE_DAMP_FACTOR) / (LOWER_FRAME_STEP - UPPER_FRAME_STEP)
#define SPHERE_DAMP_CONST SPHERE_DAMP_GRAD * UPPER_FRAME_STEP + UPPER_SPHERE_DAMP_FACTOR

int numSpheres = 0;
Sphere *spheres = NULL;

GLuint disp;

int oldX = -1, oldY = -1;
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

float zoom = 30;
float camRotateX, camRotateY;

int performanceCount = 0;
int numPhysicsThreads = 0;
bool programRunning = true;
LARGE_INTEGER *updateTimes;

boost::mutex runningLock;
boost::mutex physicsLock;

int rendering = 0;

TextField txtNumThreads, txtInstructions, txtNumSpheres, txtRenderFrames, txtPhysicsFrames;

int screenWidth = 800, screenHeight = 600;

FORCEINLINE void calcForce(Vector3 &forces, const float &springConst, const float &dampFactor, const float &overlap, const Vector3 &tangent, const Vector3 &newVelocity)
{
	Vector3 vs = tangent * dot3(tangent, newVelocity);
	forces -= (springConst * overlap * tangent) + (dampFactor * vs);
}

void boost::throw_exception(const std::exception &e)
{
}

float randf()
{
	return (float)rand() / (float)RAND_MAX;
}

int runningThreads = 0;
int pauseSimulation = 0;
int physicsRunning = 0;

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
		if(spheres == NULL)
		{
			spheres = (Sphere *)_aligned_malloc(sizeof(Sphere) * newSize, 64);
		}
		else
		{
			spheres = (Sphere *)_aligned_realloc(spheres, sizeof(Sphere) * newSize, 64);
		}
	}

	int len = newSize > numSpheres ? numSpheres : newSize;

	if(newSize > numSpheres)
	{
		for(int i = numSpheres; i < newSize; i++)
		{
			spheres[i] = Sphere();
			Sphere &sphere = spheres[i];
			
			//spheres[i].m_radius = randf() * 3.0f + 1.0f;
			//spheres[i].m_mass = spheres[i].m_radius * 5;
			//sphere.m_colour = (GLuint)(randf() * (0xFFFFFFFF));
			sphere.m_position = Vector3(randf() * ROOM_SIZE * 2.0f - ROOM_SIZE, randf() * newSize * 0.02f - (ROOM_SIZE - spheres[i].m_radius) + 20.0f, randf() * ROOM_SIZE * 2.0f - ROOM_SIZE);
			sphere.m_velocity = Vector3((randf() - 0.5f) * 10.0f, (randf() - 0.5f) * 10.0f, (randf() - 0.5f) * 10.0f);

			if(i % 3 == 0)
			{
				sphere.m_colour = 0x0000FF;
				sphere.m_radius = 0.5f;
				sphere.m_mass = 1.0f;
			}
			else if(i % 3 == 1)
			{
				sphere.m_colour = 0x00FF00;
				sphere.m_radius = 1.0f;
				sphere.m_mass = 2.0f;
			}
			else
			{
				sphere.m_colour = 0xFF0000;
				sphere.m_radius = 1.5f;
				sphere.m_mass = 3.0f;
			}
		}
	}

	numSpheres = newSize;

	char buff[32];
	sprintf(buff, "Num Spheres:    %d", numSpheres);
	txtNumSpheres.setText(buff);

	pauseSimulation = 0;
}

void init(void) 
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_COLOR_MATERIAL);

	disp = glGenLists(1);

	glNewList(disp, GL_COMPILE);
	glutSolidSphere(0.5, 12, 12);
	glEndList();
	
	QueryPerformanceFrequency( &frequency );
	freq = (double)frequency.QuadPart / 1000.0, totalTime = 0.0;

	addSpheres(STARTING_BALLS);

	ILubyte fontId = ilGenImage();
	ilBindImage(fontId);

	if(!ilLoad(IL_PNG, "default.png"))
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
		txtInstructions.setText("Press +/- to add or remove spheres.");
	}
}

void perspectiveView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

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
	
	while(pauseSimulation)
	{
		boost::this_thread::yield();
	}

	perspectiveView();

	renderFrame++;
	renderFrameCounter++;

	threadStarted();

	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 1.0f, 1.0f);
	
	for(int i = 0; i < numSpheres; i++)
	{
		glPushMatrix();
		glColor3ubv((GLubyte*)&spheres[i].m_colour);
#ifdef _SSE
		float *pos = (float*)&spheres[i].m_position.m_v128;
		glTranslatef(pos[0], pos[1], pos[2]);
#else
		glTranslatef(spheres[i].m_position.m_fX, spheres[i].m_position.m_fY, spheres[i].m_position.m_fZ);
#endif
		glScalef(spheres[i].m_radius * 2.0f, spheres[i].m_radius * 2.0f, spheres[i].m_radius * 2.0f);
		glCallList(disp);
		glPopMatrix();
	}
	
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

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void mouseFunc(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		mouseLeftDown = state == GLUT_DOWN;
		oldX = x;
		oldY = y;
	}
	if(button == GLUT_RIGHT_BUTTON)
	{
		mouseRightDown = state == GLUT_DOWN;
		oldX = x;
		oldY = y;
	}
}

void mouseMoveFunc(int x, int y)
{
	if(mouseLeftDown)
	{
		if(oldX >= 0)
		{
			int dx = x - oldX;
			int dy = y - oldY;

			camRotateX += (float)dy * 0.5f;
			camRotateY += (float)dx * 0.5f;
		}
	}
	else if(mouseRightDown)
	{
		if(oldX >= 0)
		{
			zoom += y - oldY;
		}
	}
	oldX = x;
	oldY = y;
}

void redrawLoop(int value)
{
	display();
	glutTimerFunc(UPDATE_INTERVAL, redrawLoop, 0);
}

void keyFunc(unsigned char key, int x, int y)
{
	if(key == '+')
	{
		addSpheres(1000);
	}
	else if(key == '-')
	{
		addSpheres(-1000);
	}

	cout << "Total spheres: " << numSpheres << endl;
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

		float dt = ((double)time.QuadPart - (double)updateTimes[threadNum].QuadPart) / freq;
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

		cout << "dt: " << dt << " (" << floorSpringConst << ", " << sphereSpringConst << "), (" << floorDampFactor << ", " << sphereDampFactor << ")" << endl;

		for(int i = threadNum; i < numSpheres; i += numPhysicsThreads)
		{
			Sphere &sphere = spheres[i];

			float roomSize = ROOM_SIZE - sphere.m_radius;

			Vector3 forces(0);

			Vector3 halfVelo = sphere.m_velocity + (0.5f * sphere.m_acc * dt);
			sphere.m_position += halfVelo * dt;

			Vector3 tempVelocity = sphere.m_velocity + (sphere.m_acc * dt);

			float overlap;

			for(int j = 0; j < numSpheres; j++)
			{
				if(i == j)
					continue;

				Sphere &otherSphere = spheres[j];
				float combinedRadius = sphere.m_radius + otherSphere.m_radius;
				float combinedRadiusSqrd = combinedRadius * combinedRadius;
				Vector3 toCentre = sphere.m_position - otherSphere.m_position;
				float lengthSqrd = lengthSquared(toCentre);
				float overlapSqrd = lengthSqrd - combinedRadiusSqrd;
				if(overlapSqrd < 0)
				{
					overlap = sqrt(lengthSqrd) - combinedRadius;
					Vector3 tangent = normalize(toCentre);
					calcForce(forces, sphereSpringConst, sphereDampFactor, overlap, tangent, tempVelocity);
				}
			}
			
			overlap = dot3(sphere.m_position, bottom) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, bottom, tempVelocity);
			}

			overlap = dot3(sphere.m_position, left) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, left, tempVelocity);
			}

			overlap = dot3(sphere.m_position, right) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, right, tempVelocity);
			}

			overlap = dot3(sphere.m_position, front) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, front, tempVelocity);
			}

			overlap = dot3(sphere.m_position, back) + roomSize;
			if(overlap < 0)
			{
				calcForce(forces, floorSpringConst, floorDampFactor, overlap, back, tempVelocity);
			}
			
			sphere.m_acc = forces / sphere.m_mass;
			sphere.m_acc += Vector3(0.0f, -9.8f, 0.0f);

			sphere.m_velocity = halfVelo + (0.5f * sphere.m_acc * dt);

			/*for(int j = 0; j < numSpheres; j++)
			{
				if(i == j)
					continue;



				spheres[i].collideWith2(dt, spheres[j]);
			}
			spheres[i].update(dt);*/
		}
		updateTimes[threadNum] = time;

		physicsFrameFinish();
	}
}

void frameRateFunc(int value)
{
	framesPerSecond = renderFrameCounter;
	physicsPerSecond = physicsFrameCounter;
	renderFrameCounter = 0;
	physicsFrameCounter = 0;
	glutTimerFunc(1000, frameRateFunc, 0);
}

int main(int argc, char** argv)
{
	ilInit();

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowSize (screenWidth, screenHeight); 
	glutInitWindowPosition (300, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMoveFunc);
	//glutTimerFunc(UPDATE_INTERVAL, redrawLoop, 0);
	glutTimerFunc(1000, frameRateFunc, 0);
	redrawLoop(0);

	LARGE_INTEGER firstTime;
	QueryPerformanceCounter(&firstTime);

	numPhysicsThreads = boost::thread::hardware_concurrency();
	numPhysicsThreads = 1;
	boost::thread_group threads;

	char buff[32];

	sprintf(buff, "Num threads:    %d", numPhysicsThreads);
	txtNumThreads.setText(buff);

	updateTimes = new LARGE_INTEGER[numPhysicsThreads];

	for(int i = 0; i < numPhysicsThreads; i++)
	{
		updateTimes[i] = firstTime;
		threads.create_thread(boost::bind(physicsThread, i));
	}
	
	glutMainLoop();

	programRunning = false;

	threads.join_all();

	return 0;
}