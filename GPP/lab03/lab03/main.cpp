#include "Vector3.hpp"

#include <Windows.h>
#include <iostream>
#include <xmmintrin.h>

using namespace std;

void disp(Vector3 &vec)
{
	//cout << vec.m_v128.m128_f32[0] << ", " << vec.m_v128.m128_f32[1] << ", " << vec.m_v128.m128_f32[2] << ", " << vec.m_v128.m128_f32[3] << endl;
}

int notmain()
{

	Vector3 v1(1, 2, -3);
	Vector3 v2(0, 1, 0);

	Vector3 v3 = -v1;

	disp(v3);
	system("PAUSE");
	return 0;
}