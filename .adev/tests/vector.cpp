#include "vector.h"

Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float fx, float fy)
{
	x = fx;
	y = fy;
}

Vector2 Vector2::add(const Vector2 & rhs) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::sub(const Vector2 & rhs) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator +(const Vector2 & rhs) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator -(const Vector2 & rhs) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator *(const float & rhs) const
{
	return Vector2(x * rhs, y * rhs);
}

Vector2 Vector2::operator /(const float & rhs) const
{
	return Vector2(x / rhs, y / rhs);
}
