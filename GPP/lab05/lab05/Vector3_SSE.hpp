#pragma once

#include "Vector3.h"

#include <iostream>
using namespace std;

FORCELINE Vector3::_Vector3()
{
}

FORCELINE Vector3::_Vector3(const _Vector3 &vc3Vector) : m_v128(vc3Vector.m_v128)
{
}

FORCELINE Vector3::_Vector3(const float fX, const float fY, const float fZ) : m_v128(_mm_set_ps(0, fZ, fY, fX))
{
}

FORCELINE Vector3::_Vector3(const float fFloat) : m_v128(_mm_set1_ps(fFloat))
{
}

FORCELINE float Vector3::getX() const
{
	return m_v128.m128_f32[0];
}

FORCELINE float Vector3::getY() const
{
	return m_v128.m128_f32[1];
}

FORCELINE float Vector3::getZ() const
{
	return m_v128.m128_f32[2];
}

FORCELINE void Vector3::setX(const float &fX)
{
#if _SSE4
	m_v128 = _mm_insert_ps(m_v128, _mm_set_ss(fX), _MM_MK_INSERTPS_NDX(0, 0, 0));
#else
	register __m128 v128 = _mm_movelh_ps(_mm_set_ss(fX), m_v128);
	m_v128 = _mm_shuffle_ps(v128, m_v128, _MM_SHUFFLE(3,2,3,0));
#endif
}

FORCELINE void Vector3::setY(const float &fY)
{
#if _SSE4
	m_v128 = _mm_insert_ps(m_v128, _mm_set_ss(fY), _MM_MK_INSERTPS_NDX(0, 1, 0));
#else
	register __m128 v128 = _mm_movelh_ps(m_v128, _mm_set_ss(fY));
	m_v128 = _mm_shuffle_ps(v128, m_v128, _MM_SHUFFLE(3,2,2,0));
#endif
}

FORCELINE void Vector3::setZ(const float &fZ)
{
#if _SSE4
	m_v128 = _mm_insert_ps(m_v128, _mm_set_ss(fZ), _MM_MK_INSERTPS_NDX(0, 2, 0));
#else
	m_v128 = _mm_movelh_ps(m_v128, _mm_set_ss(fZ));
#endif
}

FORCELINE Vector3::_Vector3(const __m128 &v128) : m_v128(v128)
{
}

FORCELINE Vector3 &Vector3::operator =(const Vector3 &vc3Vector)
{
	m_v128 = vc3Vector.m_v128;

	return *this;
}

FORCELINE const _Vector3 &operator +=(_Vector3 &lhs, const _Vector3 &rhs)
{
	lhs.m_v128 = _mm_add_ps(lhs.m_v128, rhs.m_v128);
	return lhs;
}

FORCELINE const _Vector3 &operator -=(_Vector3 &lhs, const _Vector3 &rhs)
{
	lhs.m_v128 = _mm_sub_ps(lhs.m_v128, rhs.m_v128);
	return lhs;
}

FORCELINE const _Vector3 &operator *=(_Vector3 &lhs, const float &rhs)
{
	lhs.m_v128 = _mm_mul_ps(lhs.m_v128, _mm_set1_ps(rhs));
	return lhs;
}

FORCELINE const _Vector3 &operator /=(_Vector3 &lhs, const float &rhs)
{
	float resp = 1.0f / rhs;
	lhs.m_v128 = _mm_mul_ps(lhs.m_v128, _mm_set1_ps(resp));
	return lhs;
}

FORCELINE float lengthSquared(const Vector3 &vc3Vector)
{
	return dot3(vc3Vector, vc3Vector);
}

FORCELINE float length(const Vector3 &vc3Vector)
{
	__m128 v;
#if _SSE4
	v = _mm_dp_ps(vc3Vector.m_v128, vc3Vector.m_v128, 0x71);
#else
	v = _mm_mul_ps(vc3Vector.m_v128, vc3Vector.m_v128);
	__m128 v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 0, 3, 2));
	v = _mm_add_ps(v, v2);
	v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 0, 1));
	v = _mm_add_ss(v, v2);
#endif
	v = _mm_sqrt_ss(v);
	return *((float*)&v);
}

FORCELINE Vector3 normalize(const Vector3 &vc3Vector)
{
	register __m128 v;
	register __m128 v2;
#if _SSE4
	v = _mm_dp_ps(vc3Vector.m_v128, vc3Vector.m_v128, 0x7F);
#else
	v = _mm_mul_ps(vc3Vector.m_v128, vc3Vector.m_v128);
	v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 0, 3, 2));
	v = _mm_add_ps(v, v2);
	v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 0, 1));
	v = _mm_add_ps(v, v2);
#endif
	v2 = _mm_rsqrt_ps(v);
	v = _mm_mul_ps(vc3Vector.m_v128, v2);

	return Vector3(v);
}

FORCELINE float dot3(const _Vector3 &lhs, const _Vector3 &rhs)
{
#if _SSE4
	__m128 v128 = _mm_dp_ps(lhs.m_v128, rhs.m_v128, 0x71);
	return *(float*)&v128;
#else
	__m128 v = _mm_mul_ps(lhs.m_v128, rhs.m_v128);
	__m128 v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 0, 3, 2));
	v = _mm_add_ps(v, v2);
	v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 0, 1));
	v = _mm_add_ss(v, v2);
	return *((float*)&v);
#endif
}

FORCELINE Vector3 cross3(const _Vector3 &lhs, const _Vector3 &rhs)
{
	register __m128 v1 = _mm_shuffle_ps(lhs.m_v128, lhs.m_v128, _MM_SHUFFLE(0, 0, 2, 1));
	register __m128 v2 = _mm_shuffle_ps(rhs.m_v128, rhs.m_v128, _MM_SHUFFLE(0, 1, 0, 2));
	register __m128 v3 = _mm_mul_ps(v1, v2);
	v1 = _mm_shuffle_ps(lhs.m_v128, lhs.m_v128, _MM_SHUFFLE(0, 1, 0, 2));
	v2 = _mm_shuffle_ps(rhs.m_v128, rhs.m_v128, _MM_SHUFFLE(0, 0, 2, 1));
	register __m128 v4 = _mm_mul_ps(v1, v2);
	return Vector3(_mm_sub_ps(v3, v4));
}

FORCELINE const Vector3 operator +(const Vector3 &vc3Vector1, const Vector3 &vc3Vector2)
{
	return Vector3(_mm_add_ps(vc3Vector1.m_v128, vc3Vector2.m_v128));
}

FORCELINE const Vector3 operator -(const Vector3 &vc3Vector1, const Vector3 &vc3Vector2)
{
	return Vector3(_mm_sub_ps(vc3Vector1.m_v128, vc3Vector2.m_v128));
}

FORCELINE const Vector3 operator *(const Vector3 &vc3Vector, const float &fFloat)
{
	return Vector3(_mm_mul_ps(vc3Vector.m_v128, _mm_set1_ps(fFloat)));
}

FORCELINE const Vector3 operator *(const float &fFloat, const Vector3 &vc3Vector)
{
	return Vector3(_mm_mul_ps(vc3Vector.m_v128, _mm_set1_ps(fFloat)));
}

FORCELINE const Vector3 operator /(const Vector3 &vc3Vector, const float &fFloat)
{
	float resp = 1.0f / fFloat;
	return Vector3(_mm_mul_ps(vc3Vector.m_v128, _mm_set1_ps(resp)));
}

FORCELINE const Vector3 operator -(const Vector3 &vc3Vector)
{
	return Vector3(_mm_mul_ps(vc3Vector.m_v128, _mm_set1_ps(-1)));
}