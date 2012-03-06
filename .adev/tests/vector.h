#pragma once

template <class T>
class Vector2
{
  public:
	/* 
	   Vector2(); Vector2(float fx, float fy); ~Vector2() {}

	   Vector2 add(const Vector2& rhs) const; Vector2 sub(const Vector2& rhs)
	   const;

	   Vector2 operator +(const Vector2 & rhs) const; Vector2 operator -(const 
	   Vector2 & rhs) const; Vector2 operator *(const float & rhs) const;
	   Vector2 operator /(const float & rhs) const; */

	Vector2()
	{
		x = 0;
		y = 0;
	}

	Vector2(T fx, T fy)
	{
		x = fx;
		y = fy;
	}

	Vector2<T> add(const Vector2<T> & rhs) const
	{
		return Vector2<T>(x + rhs.x, y + rhs.y);
	}

	Vector2<T> sub(const Vector2<T> & rhs)const
	{
		return Vector2<T>(x - rhs.x, y - rhs.y);
	}

	Vector2<T> operator +(const Vector2<T> & rhs)const
	{
		return Vector2<T>(x + rhs.x, y + rhs.y);
	}

	Vector2<T> operator -(const Vector2<T> & rhs)const
	{
		return Vector2<T>(x - rhs.x, y - rhs.y);
	}

	Vector2<T> operator *(const float &rhs)const
	{
		return Vector2<T>(x * rhs, y * rhs);
	}

	Vector2<T> operator /(const float &rhs)const
	{
		return Vector2<T>(x / rhs, y / rhs);
	}

	T x;
	T y;
};

//#include "vector.cpp"
