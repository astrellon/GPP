#define NUMVECTORS 9999999
#define NUMPASSES 10

#include <time.h>

//uses windows performance counter
#define NOMINMAX
#include <windows.h>
#include <iostream>
using namespace std;

/* setup number of vectors and number of passes to perform */

#include "benchAll.h"

#define _SSE4 1
#define _SSE2 1
#define _SSE 1

#include "Vector3.hpp"

static int perftest_SSE4()
{

	/* create a list of random vectors */
	cout << "   Building Vectors..." ;
	Vector3 * vectorList = new Vector3[NUMVECTORS];
	Vector3 * vectorList2 = new Vector3[NUMVECTORS];
	srand ( (unsigned int)time(NULL) );
	float div = (25.0f/RAND_MAX);
	for(int i=0; i<NUMVECTORS; i++)
	{
		vectorList[i] = Vector3((float)rand() * div, (float)rand() * div, (float)rand() * div);
	}
	cout << "Done" << endl;

	/* setup the performance counter we will use to check the time of each operation */
	LARGE_INTEGER frequency, timeStart, timeEnd;
	QueryPerformanceFrequency( &frequency );
	double freq = (double)frequency.QuadPart / 1000.0, totalTime = 0.0;

	/* perform desired number of passes */
	for(int j=0; j<NUMPASSES; j++)
	{
		cout << "Running Pass SSE4 " << j+1 << "...";
		QueryPerformanceCounter(&timeStart);
		for(int i=0; i<NUMVECTORS-1; i++)
		{
			/* perform series of operations */
			Vector3 temp = vectorList[i] + vectorList[i+1];
			vectorList[i] *= 3.25f;
			vectorList2[i] = vectorList[i] * length( vectorList[i] );
			vectorList2[i+1] = normalize( vectorList[i] );
			vectorList2[i] *= dot3( vectorList2[i+1], temp );
			vectorList2[i+1] = cross3( vectorList2[i], vectorList[i] );
		}
	
		/* get time taken to perform pass */
			QueryPerformanceCounter(&timeEnd);
		cout << "Done" <<  endl;
		double dTime = ((double)timeEnd.QuadPart - (double)timeStart.QuadPart)/freq;
		cout << " Time (ms): " << dTime << endl;
		totalTime += dTime;
	}

	/* get average time for each pass */
	cout << "Average: " << totalTime/(double)NUMPASSES << endl;

	/* print a random element of the vector list to prevent optimizing out */
	int IRand = (int)( (float)rand() * ((float)NUMVECTORS/(float)RAND_MAX) );
	//cout << vectorList2[IRand] << endl;
	cout << "Vector3(" << vectorList2[IRand].getX() << ", " << vectorList2[IRand].getY() << ", " << vectorList2[IRand].getZ() << ")";

	return 0;
}
