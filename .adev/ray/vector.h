#pragma once

#include <math.h>

#include <string>
#include <sstream>
#include <ostream>

using std::string;
using std::stringstream;
using std::ostream;

#ifndef _WIN32
inline int abs(int x)
{
	return abs(x);
}
#endif

template <class T>
class Vector {
public:
	Vector() : x(0), y(0), z(0), w(1) {
	}
	Vector(T x, T y, T z) :x(x), y(y), z(z), w(1) {
	}
	Vector(T x, T y, T z, T w) :x(x), y(y), z(z), w(w) {
	}
	~Vector() {}
	
	friend ostream& operator << (ostream &o, const Vector<T> &v) {
		return o << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']';
}
	
	T x;
	T y;
	T z;
	T w;
	
	inline Vector<T> add(const Vector<T> &rhs) const {
		return Vector<T>(x + rhs.x, y + rhs.y, z + rhs.z, 1);
	}
	inline Vector<T> add(T ix, T iy, T iz) const {
		return Vector<T>(x + ix, y + iy, z + iz, 1);
	}
	inline Vector<T> &addTo(const T &ix, const T &iy, const T &iz) {
		x += ix;
		y += iy;
		z += iz;
	}
	inline Vector<T> &addTo(const Vector<T> &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	inline Vector<T> sub(const Vector<T> &rhs) const {
		return Vector<T>(x - rhs.x, y - rhs.y, z - rhs.z, 1);
	}
	inline Vector<T> sub(T ix, T iy, T iz) const {
		return Vector<T>(x - ix, y - iy, z - iz, 1);
	}
	inline Vector<T> &subFrom(const T &ix, const T &iy, const T &iz) {
		x -= ix;
		y -= iy;
		z -= iz;
	}
	inline Vector<T> &subFrom(const Vector<T> &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	operator string() const
	{
		stringstream ss;
		ss << x << ", " << y << ", " << z;
		return ss.str();
	}

	operator Vector<float>() const
	{
		return Vector<float>((float)x, (float)y, (float)z, (float)w);
	}

	operator Vector<double>() const
	{
		return Vector<double>((double)x, (double)y, (double)z, (double)w);
	}

	operator Vector<long double>() const
	{
		return Vector<long double>((long double)x, (long double)y, (long double)z, (long double)w);
	}

	template <class F>
	operator Vector<F>() const
	{
		return Vector<F>((F)round(x), (F)round(y), (F)round(z), (F)round(w));
	}

	inline Vector<T> scale(double factor) const
	{
		return Vector<T>((T)(x * factor), (T)(y * factor), (T)(z * factor), 1);
	}

	inline Vector<T> multiply(const Vector<T> &rhs) const {
		return Vector<T>(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	inline Vector<T> &normalise()
	{
		double len = length();
		if(len != 0.0)
		{
			double resp = 1.0 / len;
			x = (T)(x * resp);
			y = (T)(y * resp);
			z = (T)(z * resp);
			w = (T)1; 
		}

		return *this;
	}

	inline bool equals(const Vector<T> &rhs, double tolerance = 0)
	const {
		if(tolerance > 0)
		{
			return (double)abs(rhs.x - x) <= tolerance &&
				(double)abs(rhs.y - y) <= tolerance &&
				(double)abs(rhs.z - z) <= tolerance;
		}
		else
		{
			return rhs.x == x && rhs.y == y && rhs.z == z;
		}
	}

	inline double length() const { 
		return sqrt((double)(x * x + y * y + z * z)); 
	}
	inline double lengthSqrd() const { 
		return (double)(x * x + y * y + z * z); 
	}
	inline double dot(const Vector<T> &rhs) const { 
		return (double)(x * rhs.x + y * rhs.y + z * rhs.z); 
	}
	inline double distance(const Vector<T> &rhs) const {
		double dx = x - rhs.x;
		double dy = y - rhs.y;
		double dz = z - rhs.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	inline Vector<T> &operator =(const Vector<T> &rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline Vector<T> &operator =(const float &rhs) {
		x = (T)rhs;
		y = (T)rhs;
		z = (T)rhs;
		w = (T)rhs;
		return *this;
	}

	inline Vector<T> &operator =(const double &rhs) {
		x = (T)rhs;
		y = (T)rhs;
		z = (T)rhs;
		w = (T)rhs;
		return *this;
	}

	inline Vector<T> &operator =(const long double &rhs) const {
		x = (T)rhs;
		y = (T)rhs;
		z = (T)rhs;
		w = (T)rhs;
		return *this;
	}

	inline Vector<T> &operator =(const int &rhs)
	{
		x = (T)rhs;
		y = (T)rhs;
		z = (T)rhs;
		w = (T)rhs;
		return *this;
	}
	inline bool operator ==(const Vector<T> &rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
};

typedef Vector<float> Vector4f;
typedef Vector<double> Vector4d;
typedef Vector<int> Vector4i;
