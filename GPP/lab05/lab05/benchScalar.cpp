#define NUMVECTORS 9999999
#define NUMPASSES 10

#include <time.h>

//uses windows performance counter
#define NOMINMAX
#include <windows.h>
#include <iostream>
using namespace std;

#undef _SSE4
#undef _SSE2
#undef _SSE
#define _SSE2 0
#define _SSE4 0
#define _SSE 0

#include <boost\thread\thread.hpp>

#include "Vector3.hpp"

FORCEINLINE void executeVectorsScalar(Vector3 *vectorList, Vector3 *vectorList2, int start, int finish)
{
	for(int i = start; i < finish; i++)
	{
		/* perform series of operations */
		Vector3 temp = vectorList[i] + vectorList[i+1];
		vectorList[i] *= 3.25f;
		vectorList2[i] = vectorList[i] * length( vectorList[i] );
		vectorList2[i+1] = normalize( vectorList[i] );
		vectorList2[i] *= dot3( vectorList2[i+1], temp );
		vectorList2[i+1] = cross3( vectorList2[i], vectorList[i] );
	}
}

int perftest_Scalar()
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
	cout << "Done " << _SSE << ", " << _SSE4 << endl;

	/* setup the performance counter we will use to check the time of each operation */
	LARGE_INTEGER frequency, timeStart, timeEnd;
	QueryPerformanceFrequency( &frequency );
	double freq = (double)frequency.QuadPart / 1000.0, totalTime = 0.0;

	int numThreads = boost::thread::hardware_concurrency();
	boost::thread_group threads;

#if _USE_MT
	cout << "Number of threads: " << numThreads << endl;
#endif

	float num = (float)NUMVECTORS / numThreads;

	/* perform desired number of passes */
	for(int j=0; j<NUMPASSES; j++)
	{
		cout << "Running Pass Scalar " << j+1 << "...";

		QueryPerformanceCounter(&timeStart);
#if _USE_MT

		for(int t = 0; t < numThreads; t++)
		{
			int finish = (int)((float)(t + 1) * num);
			if(t == numThreads - 1)
				finish--;
			threads.create_thread(boost::bind(executeVectorsScalar, vectorList, vectorList2, (int)(t * num), finish));
		}
		threads.join_all();
#else
		for(int i=0; i<NUMVECTORS-1; i++)
		{
			
			Vector3 temp = vectorList[i] + vectorList[i+1];
			vectorList[i] *= 3.25f;
			vectorList2[i] = vectorList[i] * length( vectorList[i] );
			vectorList2[i+1] = normalize( vectorList[i] );
			vectorList2[i] *= dot3( vectorList2[i+1], temp );
			vectorList2[i+1] = cross3( vectorList2[i], vectorList[i] );
		}
#endif
	
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
