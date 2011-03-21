#pragma once

typedef struct _Vector3
{
	float m_fX, m_fY, m_fZ, m_fW;

	_Vector3();

	_Vector3( const _Vector3 &vc3Vector );

	_Vector3( const float fX, const float fY, const float fZ);

	explicit _Vector3(const float fFloat);

	_Vector3 &operator = (const _Vector3 &vc3Vector);
} Vector3;

float lengthSquared(const Vector3 &vc3Vector);
float length(const Vector3 &vc3Vector);
Vector3 normalize(const Vector3 &vc3Vector);
float dot3(const _Vector3 &lhs, const _Vector3 &rhs);
Vector3 cross3(const _Vector3 &lhs, const _Vector3 &rhs);

static const Vector3 operator +(const Vector3 &vc3Vector1, const Vector3 &vc3Vector2);
static const Vector3 operator -(const Vector3 &vc3Vector1, const Vector3 &vc3Vector2);
static const Vector3 operator *(const Vector3 &vc3Vector, const float &fFloat);
static const Vector3 operator *(const float &fFloat, const Vector3 &vc3Vector);
static const Vector3 operator /(const Vector3 &vc3Vector, const float &fFloat);
static const Vector3 operator -(const Vector3 &vc3Vector);

static const _Vector3 &operator +=(_Vector3 &lhs, const _Vector3 &rhs);
static const _Vector3 &operator -=(_Vector3 &lhs, const _Vector3 &rhs);
static const _Vector3 &operator *=(_Vector3 &lhs, const float &rhs);
static const _Vector3 &operator /=(_Vector3 &lhs, const float &rhs);