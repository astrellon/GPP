#pragma once

#include "vector.h"
#include "material.h"
#include "irenderable.h"

#include <iostream>
using  namespace std;

class Sphere : public IRenderable {
public:
	Sphere() {
	}
	Sphere(float size) {
		this->size = size;
	}

	virtual bool emitsLight() const {
		return false;
	}
	
	virtual bool collides(const Ray &r, RayHitResult &result) const { 
		//Compute A, B and C coefficients 
		//Vector4f start = transform.getWorldToObj().translateVectorConst(r.getStart());
		Vector4f start = r.getStart().add(transform.getPosition());
		//cout << "Ray start: " << start << '\n'; 
		Vector4f dir = r.getDir();
		float a = static_cast<float>(dir.dot(dir));
		float b = 2.0f * static_cast<float>(dir.dot(start));
		float c = static_cast<float>(start.dot(start)) - (size * size);

		//Find discriminant 
		float disc = b * b - 4 * a * c;

		// if discriminant is negative there are no real roots, so 	return 
		// false as ray misses sphere 
		if (disc < 0) return false;
	
		// compute q as described above 
		float distSqrt = sqrtf(disc); 
		float q; 
		if (b < 0) 
			q = (-b - distSqrt) * 0.5f; 
		else 
			q = (-b + distSqrt) * 0.5f;

		// compute t0 and t1 
		float t0 = q / a; 
		float t1 = c / q;

		// make sure t0 is smaller than t1 
		if (t0 > t1) { 
			// if t0 is bigger than t1 swap them around 
			float temp = t0; 
			t0 = t1; 
			t1 = temp; 
		}

		// if t1 is less than zero, the object is in the ray's negative direction 
		// and consequently the ray misses the sphere 
		if (t1 < 0) return false;

		result.target = this;
		// if t0 is less than zero, the intersection point is at t1 
		
		float len = 0;
		if (t0 < 0) {
			len = t1;
		} // else the intersection point is at t0 
		else {
			len = t0;
		}
		result.len = len;
		Vector4f ss = r.getStart();
		result.worldPos = ss.add(r.getDir().scale(len));
		result.normal = start.add(dir.scale(len)).normalise();
		
		return true;
	}
	
	float size;
};
