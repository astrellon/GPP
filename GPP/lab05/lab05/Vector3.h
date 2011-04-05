#pragma once

#if _SSE
#	include <xmmintrin.h>
#endif
#if _SSE2
#	include <emmintrin.h>
#endif
#if _SSE4
#	include <smmintrin.h>
#endif

typedef struct _Vector3
{
#if _SSE
	__m128 m_v128;
#else
	float m_fX, m_fY, m_fZ;
#endif

	_Vector3();

	_Vector3( const _Vector3 &vc3Vector );

	_Vector3( const float fX, const float fY, const float fZ);

	explicit _Vector3(const float fFloat);

	float getX() const;
	float getY() const;
	float getZ() const;

	void setX(const float &fX);
	void setY(const float &fY);
	void setZ(const float &fZ);

#if _SSE
	explicit _Vector3(const __m128 &v128);
#endif

	_Vector3 &operator = (const _Vector3 &vc3Vector);
} Vector3;

static float lengthSquared(const Vector3 &vc3Vector);
static float length(const Vector3 &vc3Vector);
static Vector3 normalize(const Vector3 &vc3Vector);
static float dot3(const _Vector3 &lhs, const _Vector3 &rhs);
static Vector3 cross3(const _Vector3 &lhs, const _Vector3 &rhs);

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