#include "Vector3.hpp"

#include <Windows.h>
#include <iostream>
#include <xmmintrin.h>

using namespace std;

void disp(Vector3 &vec)
{
	cout << vec.m_v128.m128_f32[0] << ", " << vec.m_v128.m128_f32[1] << ", " << vec.m_v128.m128_f32[2] << ", " << vec.m_v128.m128_f32[3] << endl;
}

int main()
{
	Vector3 v(5, 5, 5);

	cout << "Length: " << length(v) << endl;
	
	v = normalize(v);

	disp(v);
	system("PAUSE");
	return 0;
}