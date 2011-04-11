#include <Windows.h>

#include "glut.h"
#include <gl\gl.h>
#include <gl\glu.h>

#include <stdlib.h>

#include <boost\thread\thread.hpp>

#include <time.h>

#include <iostream>

using namespace std;

#include "Sphere.h"

#define UPDATE_INTERVAL 20

int numSpheres = 0;
Sphere *spheres = NULL;

GLuint disp;

float roomSize = 20.0f;
int oldX = -1, oldY = -1;

LARGE_INTEGER frequency, timeStart, timeEnd;
double freq, totalTime;

bool mouseLeftDown = false;
bool mouseRightDown = false;

float zoom = 30;
float camRotateX;
float camRotateY;

int performanceCount = 0;

int numPhysicsThreads = 0;

bool programRunning = true;

LARGE_INTEGER *updateTimes;

boost::mutex runningLock;

int rendering = 0;

#if _RELEASE
void boost::throw_exception(const std::exception &e)
{
}
#endif

float randf()
{
	return (float)rand() / (float)RAND_MAX;
}

int runningThreads = 0;
int pauseSimulation = 0;

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
	Sphere *newArray;
	int newSize = numSpheres + num;
	if(newSize <= 0)
	{
		newSize = 0;
	}
	else
	{
		newArray = new Sphere[newSize];
	}

	//Sphere *newArray = (Sphere *)malloc(sizeof(Sphere) * newSize);

	int len = newSize > numSpheres ? numSpheres : newSize;

	for(int i = 0; i < len; i++)
	{
		newArray[i] = spheres[i];
	}

	if(newSize > numSpheres)
	{
		for(int i = numSpheres; i < newSize; i++)
		{
			newArray[i].m_position = Vector3(randf() * roomSize * 2.0f - roomSize, randf() * newSize * 0.02f - roomSize, randf() * roomSize * 2.0f - roomSize);
			newArray[i].m_velocity = Vector3(0);
			//newArray[i].m_velocity = Vector3(randf() * roomSize * 2.0f - roomSize, randf() * roomSize * 2.0f - roomSize, randf() * roomSize * 2.0f - roomSize);
			newArray[i].m_radius = randf() * 1.0f + 0.5f;
			newArray[i].m_mass = newArray[i].m_radius * 5;
			newArray[i].m_colour = (GLuint)(randf() * (0xFFFFFFFF));
		}
	}

	numSpheres = newSize;

	delete []spheres;
	//free(spheres);
	spheres = newArray;

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
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);

	disp = glGenLists(1);

	glNewList(disp, GL_COMPILE);
	glutSolidSphere(0.5, 12, 12);
	glEndList();
	
	QueryPerformanceFrequency( &frequency );
	freq = (double)frequency.QuadPart / 1000.0, totalTime = 0.0;

	addSpheres(500);
	/*
	for(int i = 0; i < NUM_SPHERES; i++)
	{
		spheres[i].m_position = Vector3(randf() * roomSize * 2.0f - roomSize, randf() * NUM_SPHERES * 0.02f - roomSize, randf() * roomSize * 2.0f - roomSize);
		spheres[i].m_velocity = Vector3(randf() * roomSize * 2.0f - roomSize, randf() * roomSize * 2.0f - roomSize, randf() * roomSize * 2.0f - roomSize);

		

		//spheres[i].m_radius = randf() * 1.0f + 0.5f;
		//spheres[i].m_mass = spheres[i].m_radius * 5;
		//spheres[i].m_red = spheres[i].m_radius / 3.0f;
		//spheres[i].m_colour = (GLuint)(randf() * (0xFFFFFFFF));
	}*/
	/*
	spheres[0].m_mass = 2.0;
	spheres[0].m_position = Vector3(-5.0f, 0.0f, 0.0f);
	spheres[0].m_velocity = Vector3(0.0f, 0.0f, 0);
	spheres[0].m_red = 0.3;
	spheres[1].m_position = Vector3(5.0f, 0.0f, 0.0f);
	spheres[1].m_velocity = Vector3(-1.0f, 0, 0);*/
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslated(0, 0, -zoom);
	glRotatef(camRotateX, 1, 0, 0);
	glRotatef(camRotateY, 0, 1, 0);
	
	while(pauseSimulation)
	{
		boost::this_thread::yield();
	}

	threadStarted();
	
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
	//glPopMatrix();

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	GLfloat zNear = 0.1f;
	GLfloat zFar = 255.0f;
	GLfloat aspect = float(w)/float(h);
	GLfloat fH = tan( float(65.0f / 360.0f * 3.14159f) ) * zNear;
	GLfloat fW = fH * aspect;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void updateLoop(int value)
{
	if(performanceCount == 0)
	{
		QueryPerformanceCounter(&timeStart);
	}
	performanceCount++;
	/*for(int i = 0; i < NUM_SPHERES - 1; i++)
	{
		for(int j = i + 1; j < NUM_SPHERES; j++)
		{
			spheres[i].collideWith(0.04f, spheres[j]);
		}
	}*/
/*
	for(int i = 0; i < NUM_SPHERES; i++)
	{
		for(int j = 0; j < NUM_SPHERES; j++)
		{
			if(i == j)
				continue;
			spheres[i].collideWith2(0.04f, spheres[j]);
		}
	}
	for(int i = 0; i < NUM_SPHERES; i++)
	{
		spheres[i].update(0.04f);
	}

	if(performanceCount == 10)
	{
		QueryPerformanceCounter(&timeEnd);
		double dTime = ((double)timeEnd.QuadPart - (double)timeStart.QuadPart)/freq;
		//cout << " Time (ms): " << dTime << endl;
		performanceCount = 0;
	}
	*/
	//glutPostRedisplay();
	//glutTimerFunc(UPDATE_INTERVAL, updateLoop, 0);
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
	//cout << lower << ", " << upper << endl;

	int physicsFrame = 0;
	//int renderingFrame = -1;

	int threadRunning = 0;

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
		float num = (float)numSpheres / (float)numPhysicsThreads;
		int lower = (int)(threadNum * num);
		int upper = (int)((threadNum + 1) * num);
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		float dt = ((double)time.QuadPart - (double)updateTimes[threadNum].QuadPart) / freq;
		dt *= 0.001;
		for(int i = lower; i < upper; i++)
		{
			for(int j = 0; j < numSpheres; j++)
			{
				if(i == j)
					continue;
				spheres[i].collideWith2(dt, spheres[j]);
			}
		}
		/*
		while(rendering && renderingFrame < physicsFrame)
		{
			renderLock.lock();
			renderLock.unlock();
		}
		*/

		//if(!rendering)
		//{
			//renderLock.lock();
			//renderingFrame = physicsFrame;
			for(int i = lower; i < upper; i++)
			{
				spheres[i].update(dt);
			}
			//renderLock.unlock();
		//}
		physicsFrame ++;

		updateTimes[threadNum] = time;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMoveFunc);
	glutTimerFunc(UPDATE_INTERVAL, redrawLoop, 0);

	LARGE_INTEGER firstTime;
	QueryPerformanceCounter(&firstTime);

	numPhysicsThreads = boost::thread::hardware_concurrency();
	//numPhysicsThreads = 1;
	boost::thread_group threads;
	//int num = NUM_SPHERES / numPhysicsThreads;

	updateTimes = new LARGE_INTEGER[numPhysicsThreads];

	for(int i = 0; i < numPhysicsThreads; i++)
	{
		updateTimes[i] = firstTime;
		threads.create_thread(boost::bind(physicsThread, i));
		//threads.create_thread(boost::bind(physicsThread, i, i * num, (i + 1) * num));
		//physicsThreads[i] = boost::thread(physicsThread, i, i * num, (i + 1) * num);
		
	}
	
	glutMainLoop();

	programRunning = false;

	threads.join_all();
	/*for(int i = 0; i < numPhysicsThreads; i++)
	{
		physicsThreads[i].join();
		
	}*/
	return 0;
}