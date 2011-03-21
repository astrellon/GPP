#pragma once

#include "Vector3.h"
#include <math.h>

FORCELINE Vector3::_Vector3()
{
}

FORCELINE Vector3::_Vector3(const _Vector3 &vc3Vector) : m_fX(vc3Vector.m_fX), m_fY(vc3Vector.m_fY), m_fZ(vc3Vector.m_fZ)
{
}

FORCELINE Vector3::_Vector3(const float fX, const float fY, const float fZ) : m_fX(fX), m_fY(fY), m_fZ(fZ)
{
}

FORCELINE Vector3::_Vector3(const float fFloat) : m_fX(fFloat), m_fY(fFloat), m_fZ(fFloat)
{
}

FORCELINE _Vector3 &Vector3::operator =(const _Vector3 &vc3Vector)
{
	m_fX = vc3Vector.m_fX;
	m_fY = vc3Vector.m_fY;
	m_fZ = vc3Vector.m_fZ;

	return *this;
}

FORCELINE static const _Vector3 &operator +=(_Vector3 &lhs, const _Vector3 &rhs)
{
	lhs.m_fX += rhs.m_fX;
	lhs.m_fY += rhs.m_fY;
	lhs.m_fZ += rhs.m_fZ;

	return lhs;
}

FORCELINE static const _Vector3 &operator -=(_Vector3 &lhs, const _Vector3 &rhs)
{
	lhs.m_fX -= rhs.m_fX;
	lhs.m_fY -= rhs.m_fY;
	lhs.m_fZ -= rhs.m_fZ;

	return lhs;
}

FORCELINE static const _Vector3 &operator *=(_Vector3 &lhs, const float &rhs)
{
	lhs.m_fX *= rhs;
	lhs.m_fY *= rhs;
	lhs.m_fZ *= rhs;

	return lhs;
}

FORCELINE static const _Vector3 &operator /=(_Vector3 &lhs, const float &rhs)
{
	float resp = 1.0f / rhs;
	lhs.m_fX *= resp;
	lhs.m_fY *= resp;
	lhs.m_fZ *= resp;

	return lhs;
}

FORCELINE float lengthSquared(const Vector3 &vc3Vector)
{
	return vc3Vector.m_fX * vc3Vector.m_fX + vc3Vector.m_fY * vc3Vector.m_fY + vc3Vector.m_fZ * vc3Vector.m_fZ;
}

FORCELINE float length(const Vector3 &vc3Vector)
{
	return sqrt(vc3Vector.m_fX * vc3Vector.m_fX + vc3Vector.m_fY * vc3Vector.m_fY + vc3Vector.m_fZ * vc3Vector.m_fZ);
}

FORCELINE Vector3 normalize(const Vector3 &vc3Vector)
{
	return vc3Vector / length(vc3Vector);
}

FORCELINE float dot3(const _Vector3 &lhs, const _Vector3 &rhs)
{
	return lhs.m_fX * rhs.m_fX + lhs.m_fY * rhs.m_fY + lhs.m_fZ * rhs.m_fZ;
}

FORCELINE Vector3 cross3(const _Vector3 &lhs, const _Vector3 &rhs)
{
	return Vector3(
		lhs.m_fY * rhs.m_fZ - lhs.m_fZ * rhs.m_fY,
		lhs.m_fZ * rhs.m_fX - lhs.m_fX * rhs.m_fZ,
		lhs.m_fX * rhs.m_fY - lhs.m_fY * rhs.m_fX);
}

FORCELINE static const Vector3 operator +(const Vector3 &vc3Vector1, const Vector3 &vc3Vector2)
{
	return Vector3(vc3Vector1.m_fX + vc3Vector2.m_fX, vc3Vector1.m_fY + vc3Vector2.m_fY, vc3Vector1.m_fZ + vc3Vector2.m_fZ);
}

FORCELINE static const Vector3 operator -(const Vector3 &vc3Vector1, const Vector3 &vc3Vector2)
{
	return Vector3(vc3Vector1.m_fX - vc3Vector2.m_fX, vc3Vector1.m_fY - vc3Vector2.m_fY, vc3Vector1.m_fZ - vc3Vector2.m_fZ);
}

FORCELINE static const Vector3 operator *(const Vector3 &vc3Vector, const float &fFloat)
{
	return Vector3(vc3Vector.m_fX * fFloat, vc3Vector.m_fY * fFloat, vc3Vector.m_fZ * fFloat);
}

FORCELINE static const Vector3 operator *(const float &fFloat, const Vector3 &vc3Vector)
{
	return Vector3(vc3Vector.m_fX * fFloat, vc3Vector.m_fY * fFloat, vc3Vector.m_fZ * fFloat);
}

FORCELINE static const Vector3 operator /(const Vector3 &vc3Vector, const float &fFloat)
{
	float resp = 1.0f / fFloat;
	return Vector3(vc3Vector.m_fX * resp, vc3Vector.m_fY * resp, vc3Vector.m_fZ * resp);
}

FORCELINE static const Vector3 operator -(const Vector3 &vc3Vector)
{
	return Vector3(-vc3Vector.m_fX, -vc3Vector.m_fY, -vc3Vector.m_fZ);
}