#pragma once

#include "vector.h"
#include "matrix.h"
#include "iscene_element.h"
#include "ray.h"

class Camera : public ISceneElement {
public:
	Camera() {
		resX = 40;
		resY = 30;
		aspect = 4.0 / 3.0;
	}
	~Camera() {}
	
	Ray getRay(int x, int y) {
		Vector4f start;
		Vector4f dir(0, 0, 1);
		if (ortho) {
			y = resY - y;
			x = resX - x;
			float rx = (float)x / (float)resX - 0.5f;
			float ry = (float)y / (float)resY - 0.5f;
			rx *= aperture * aspect;
			ry *= aperture;
			
			start.x = rx;
			start.y = ry;
		}
		else {
		
		}
		transform.transformVectorConst(dir);
		transform.transformVectorConst(start);
		transform.translateVector(start);
		
		return Ray(start, dir);
	}
	
	inline void flipZ() {
		transform.xz = -transform.xz;
		transform.yz = -transform.yz;
		transform.zz = -transform.zz;
	}
	
	inline bool getOrtho() const {
		return ortho;
	}
	inline void setOrtho(bool o) {
		ortho = o;
	}
	
	inline float getAperture() const {
		return aperture;
	}
	inline void setAperture(float a) {
		aperture = a;
	}
	
	inline int getResX() const {
		return resX;
	}
	inline int getResY() const {
		return resY;
	}
	
	inline void setRes(int x, int y) {
		if (y <= 0) {
			y = 1;
		}
		resX = x;
		resY = y;
		aspect = (float)x / (float)y;
	}
	
	virtual bool emitsLight() const {
		return false;
	}
	virtual bool isCollidable() const {
		return false;
	}
	
protected:
	bool ortho;
	float aperture;
	
	int resX;
	int resY;
	float aspect;
};
