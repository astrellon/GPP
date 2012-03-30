#pragma once

#include "vector.h"

class Ray {
public:
	Ray() {}
	Ray(const Vector4f start, const Vector4f dir) :
		start(start), dir(dir) {
	}
	
	inline Vector4f getStart() const {
		return start;
	}
	inline Vector4f getDir() const {
		return dir;
	}

	int bounceNum;
	Vector4f colour;
	
protected:
	Vector4f start;
	Vector4f dir;
};
