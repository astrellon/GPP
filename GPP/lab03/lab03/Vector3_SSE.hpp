#pragma once

#include "Vector3.h"

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

FORCELINE Vector3::_Vector3(const __m128 &v128) : m_v128(v128)
{
}

FORCELINE _Vector3 &Vector3::operator =(const _Vector3 &vc3Vector)
{
	m_v128 = vc3Vector.m_v128;

	return *this;
}

FORCELINE static const _Vector3 &operator +=(_Vector3 &lhs, const _Vector3 &rhs)
{
	lhs.m_v128 = _mm_add_ps(lhs.m_v128, rhs.m_v128);
	return lhs;
}

FORCELINE static const _Vector3 &operator -=(_Vector3 &lhs, const _Vector3 &rhs)
{
	lhs.m_v128 = _mm_sub_ps(lhs.m_v128, rhs.m_v128);
	return lhs;
}

FORCELINE static const _Vector3 &operator *=(_Vector3 &lhs, const float &rhs)
{
	lhs.m_v128 = _mm_mul_ps(lhs.m_v128, _mm_set1_ps(rhs));
	return lhs;
}

FORCELINE static const _Vector3 &operator /=(_Vector3 &lhs, const float &rhs)
{
	float resp = 1.0f / rhs;
	lhs.m_v128 = _mm_mul_ps(lhs.m_v128, _mm_set1_ps(resp));
	return lhs;
}

FORCELINE float lengthSquared(const Vector3 &vc3Vector)
{
	return dot(vc3Vector.m_v128, vc3Vector.m_v128);
}

FORCELINE float length(const Vector3 &vc3Vector)
{
	__m128 v = _mm_mul_ps(vc3Vector.m_v128, vc3Vector.m_v128);
	__m128 v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 0, 3, 2));
	v = _mm_add_ps(v, v2);
	v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 0, 1));
	v = _mm_add_ss(v, v2);
	v = _mm_sqrt_ss(v);
	return *((float*)&v);
}

FORCELINE Vector3 normalize(const Vector3 &vc3Vector)
{
	__m128 v = _mm_mul_ps(vc3Vector.m_v128, vc3Vector.m_v128);
	__m128 v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 0, 3, 2));
	v = _mm_add_ps(v, v2);
	v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 0, 1));
	v = _mm_add_ps(v, v2);
	v2 = _mm_rsqrt_ps(v);
	v = _mm_mul_ps(vc3Vector.m_v128, v2);

	return Vector3(v);
}

FORCELINE float dot3(const _Vector3 &lhs, const _Vector3 &rhs)
{
	__m128 v = _mm_mul_ps(lhs.m_v128, rhs.m_v128);
	__m128 v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 0, 3, 2));
	v = _mm_add_ps(v, v2);
	v2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 0, 1));
	v = _mm_add_ss(v, v2);
	return *((float*)&v);
}

FORCELINE Vector3 cross3(const _Vector3 &lhs, const _Vector3 &rhs)
{
	//__m128 v1 = _mm_shuffle_ps(lhs, rhs, _MM_SHUFFLE(
	return Vector3();
	//return Vector3(
//		lhs.m_fY * rhs.m_fZ - lhs.m_fZ * rhs.m_fY,
		//lhs.m_fZ * rhs.m_fX - lhs.m_fX * rhs.m_fZ,
		//lhs.m_fX * rhs.m_fY - lhs.m_fY * rhs.m_fX);
}

FORCELINE static const Vector3 operator +(const Vector3 &vc3Vector1, const Vector3 &vc3Vector2)
{
	return Vector3(_mm_add_ps(vc3Vector1.m_v128, vc3Vector2.m_v128));
}

FORCELINE static const Vector3 operator -(const Vector3 &vc3Vector1, const Vector3 &vc3Vector2)
{
	return Vector3(_mm_sub_ps(vc3Vector1.m_v128, vc3Vector2.m_v128));
}

FORCELINE static const Vector3 operator *(const Vector3 &vc3Vector, const float &fFloat)
{
	return Vector3(_mm_mul_ps(vc3Vector.m_v128, _mm_set1_ps(fFloat)));
}

FORCELINE static const Vector3 operator *(const float &fFloat, const Vector3 &vc3Vector)
{
	return Vector3(_mm_mul_ps(vc3Vector.m_v128, _mm_set1_ps(fFloat)));
}

FORCELINE static const Vector3 operator /(const Vector3 &vc3Vector, const float &fFloat)
{
	float resp = 1.0f / fFloat;
	return Vector3(_mm_mul_ps(vc3Vector.m_v128, _mm_set1_ps(resp)));
}

FORCELINE static const Vector3 operator -(const Vector3 &vc3Vector)
{
	return Vector3(_mm_mul_ps(vc3Vector.m_v128, _mm_set1_ps(-1)));
}