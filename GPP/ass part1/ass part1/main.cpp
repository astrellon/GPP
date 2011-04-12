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

int numSpheres = 0;
Sphere *spheres = NULL;

GLuint disp;

int oldX = -1, oldY = -1;
int renderFrame = 0;
int physicsFrame = 0;

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
			spheres[i].m_position = Vector3(randf() * ROOM_SIZE * 2.0f - ROOM_SIZE, randf() * newSize * 0.02f - ROOM_SIZE, randf() * ROOM_SIZE * 2.0f - ROOM_SIZE);
			spheres[i].m_velocity = Vector3(0);
			spheres[i].m_radius = randf() * 3.0f + 1.0f;
			spheres[i].m_mass = spheres[i].m_radius * 5;
			spheres[i].m_colour = (GLuint)(randf() * (0xFFFFFFFF));
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

	addSpheres(500);

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
	
	while(pauseSimulation)
	{
		boost::this_thread::yield();
	}

	perspectiveView();

	renderFrame++;

	threadStarted();

	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 1.0f, 1.0f);
	
	//glPushMatrix();
	for(int i = 0; i < numSpheres; i++)
	{
		glPushMatrix();
		//glColor3f(spheres[i].m_red, 0.2f, 0.4f);
		glColor3bv((GLbyte*)&spheres[i].m_colour);
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

	char buff[32];

	sprintf(buff, "Render frames:  %d", renderFrame);
	txtRenderFrames.setText(buff);

	sprintf(buff, "Physics frames: %d", physicsFrame);
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
	//glutPostRedisplay();
	display();
	glutTimerFunc(UPDATE_INTERVAL, redrawLoop, 0);
}

void keyFunc(unsigned char key, int x, int y)
{
	if(key == '+')
	{
		addSpheres(100);
	}
	else if(key == '-')
	{
		addSpheres(-100);
	}

	cout << "Total spheres: " << numSpheres << endl;
}

void physicsThread(int threadNum)
{
	int threadRunning = 0;
	int localFrame = 0;

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

		float num = (float)numSpheres / (float)numPhysicsThreads;
		int lower = (int)(threadNum * num);
		int upper = (int)((threadNum + 1) * num);
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		float dt = ((double)time.QuadPart - (double)updateTimes[threadNum].QuadPart) / freq;
		dt *= 0.001;

		for(int i = threadNum; i < numSpheres; i += numPhysicsThreads)
		{
			for(int j = 0; j < numSpheres; j++)
			{
				if(i == j)
					continue;
				spheres[i].collideWith2(dt, spheres[j]);
			}
			spheres[i].update(dt);
		}
		/*
		for(int i = threadNum; i < numSpheres; i += numPhysicsThreads)
		{
			spheres[i].update(dt);
		}
		*/
		updateTimes[threadNum] = time;

		physicsFrameFinish();
	}
}

int main(int argc, char** argv)
{
	ilInit();

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowSize (screenWidth, screenHeight); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMoveFunc);
	//glutTimerFunc(UPDATE_INTERVAL, redrawLoop, 0);
	redrawLoop(0);

	LARGE_INTEGER firstTime;
	QueryPerformanceCounter(&firstTime);

	numPhysicsThreads = boost::thread::hardware_concurrency();
	//numPhysicsThreads = 1;
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