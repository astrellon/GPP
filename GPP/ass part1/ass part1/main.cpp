#include <Windows.h>

#include "glut.h"
#include <gl\gl.h>
#include <gl\glu.h>

#include <boost\thread\thread.hpp>

#include <time.h>

#include <iostream>

using namespace std;

#include "Sphere.h"

#define NUM_SPHERES 2000
#define UPDATE_INTERVAL 40

Sphere spheres[NUM_SPHERES];

GLuint disp;

float roomSize = 20.0f;
int oldX = -1, oldY = -1;

LARGE_INTEGER frequency, timeStart, timeEnd;
double freq, totalTime;

bool mouseLeftDown = false;
bool mouseRightDown = false;

float zoom = 30;
//Vector3 camRotate;
float camRotateX;
float camRotateY;

int performanceCount = 0;

int numPhysicsThreads = 4;
boost::thread *physicsThreads;

bool programRunning = true;

LARGE_INTEGER updateTimes[4];

void boost::throw_exception(const std::exception &e)
{
}

float randf()
{
	return (float)rand() / (float)RAND_MAX;
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
	
	for(int i = 0; i < NUM_SPHERES; i++)
	{
		spheres[i].m_position = Vector3(randf() * roomSize * 2.0f - roomSize, randf() * NUM_SPHERES * 0.02f - roomSize, randf() * roomSize * 2.0f - roomSize);
		spheres[i].m_velocity = Vector3(randf() * roomSize * 2.0f - roomSize, randf() * roomSize * 2.0f - roomSize, randf() * roomSize * 2.0f - roomSize);

		spheres[i].m_radius = randf() * 1.0f + 0.5f;
		spheres[i].m_mass = spheres[i].m_radius * 5;
		spheres[i].m_red = spheres[i].m_radius / 3.0f;
	}
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

	//glPushMatrix();
	for(int i = 0; i < NUM_SPHERES; i++)
	{
		glPushMatrix();
		glColor3f(spheres[i].m_red, 0.2f, 0.4f);
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
	glutPostRedisplay();
	glutTimerFunc(UPDATE_INTERVAL, redrawLoop, 0);
}

void physicsThread(int threadNum, int lower, int upper)
{
	cout << lower << ", " << upper << endl;

	while(programRunning)
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		float dt = ((double)time.QuadPart - (double)updateTimes[threadNum].QuadPart) / freq;
		dt *= 0.001;
		for(int i = lower; i < upper; i++)
		{
			for(int j = 0; j < NUM_SPHERES; j++)
			{
				if(i == j)
					continue;
				spheres[i].collideWith2(dt, spheres[j]);
			}
		}
		for(int i = lower; i < upper; i++)
		{
			spheres[i].update(dt);
		}

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
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMoveFunc);
	glutTimerFunc(UPDATE_INTERVAL, redrawLoop, 0);

	int num = NUM_SPHERES / numPhysicsThreads;

	physicsThreads = new boost::thread[numPhysicsThreads];

	LARGE_INTEGER firstTime;
	QueryPerformanceCounter(&firstTime);

	for(int i = 0; i < numPhysicsThreads; i++)
	{
		updateTimes[i] = firstTime;
		physicsThreads[i] = boost::thread(physicsThread, i, i * num, (i + 1) * num);
	}
	
	glutMainLoop();

	programRunning = false;
	for(int i = 0; i < numPhysicsThreads; i++)
	{
		physicsThreads[i].join();
	}
	return 0;
}