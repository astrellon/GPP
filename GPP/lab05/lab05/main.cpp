#include <Windows.h>
#include <iostream>

#if defined (_MSC_VER) || defined (__INTEL_COMPILER)
#	include<intrin.h>

void cpuid(int iType, int iOutput[4])
{
	__cpuid(iOutput, iType);
}
#elif defined(__GNUC__) || defined(__SUNPRO_C)
void cpuid(int iType, int iOutput[4])
{
	__asm__ __volatile__ ("cpuid": "=a" (iOutput[0]), "=b" (iOutput[1]),
		"=c" (iOutput[2]), "=d" (iOutput[3]) : "a" (iType) );
}
#else
#	error Unrecognized compiler
#endif

using namespace std;

int perftest_SSE4();
int perftest_SSE();
int perftest_Scalar();

int main()
{
	int iCPUVals[4];
	cpuid(1, iCPUVals);
	bool sse4Support = false;
	bool sseSupport = false;
	if(iCPUVals[3] & (1 << 25))
	{
		cout << "SSE supported" << endl;
#if USE_SSE
		sseSupport = true;
#endif
	}
	if(iCPUVals[3] & (1 << 26))
	{
		cout << "SSE2 supported" << endl;
	}
	if(iCPUVals[2] & (1 << 0))
	{
		cout << "SSE3 supported" << endl;
	}
	if(iCPUVals[2] & (1 << 9))
	{
		cout << "SSSE3 supported" << endl;
	}
	if(iCPUVals[2] & (1 << 19))
	{
#if USE_SSE4
		sse4Support = true;
#endif
		cout << "SSE4.1 supported" << endl;
	}
	if(iCPUVals[2] & (1 << 20))
	{
		cout << "SSE4.2 supported" << endl;
	}
	if(iCPUVals[2] & (1 << 28))
	{
		cout << "AVX supported" << endl;
	}
	cout << "--------------" << endl << endl;
	if(sse4Support)
	{
		cout << "Using SSE4 bench code" << endl;
		perftest_SSE4();
	}
	else if(sseSupport)
	{
		cout << "Using SSE bench code" << endl;
		perftest_SSE();
	}
	else
	{
		cout << "Using Scalar bench code" << endl;
		perftest_Scalar();
	}

	cout << endl;
	system("PAUSE");
	return 0;
}