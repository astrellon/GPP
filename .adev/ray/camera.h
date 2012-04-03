#pragma once

#include "vector.h"
#include "matrix.h"
#include "iscene_element.h"
#include "ray.h"
#include "math.h"

class Camera : public ISceneElement {
public:
	Camera() {
		resX = 40;
		resY = 30;
		aspect = 4.0f / 3.0f;
		fov = M_PI * 0.5f;
	}
	~Camera() {}
	
	Ray getRay(int x, int y) {
		Vector4f start;
		Vector4f dir(0, 0, 1);
		
		y = resY - y;
		x = resX - x;
		float rx = (float)x / (float)resX - 0.5f;
		float ry = (float)y / (float)resY - 0.5f;
		rx *= fov * aspect;
		ry *= fov;
		
		if (ortho) {
			start.x = rx;
			start.y = ry;
			
		cout << "Firing ray (" << x << ',' << y << ") " << start << '\n';
		}
		else {
			
			//dir.x = 1.0f - sqrt(1 - rx * rx);
			//dir.y = 1.0f - sqrt(1 - ry * ry);
			//dir.x = x - (float)resX * 0.5f;
			//dir.y = y - (float)resY * 0.5f;
			//dir.normalise();
			printf("(%d, %d) %f, %f\n", x, y, dir.x, dir.y);
		}
		
		Matrix4f &m = transform.getWorldToObj();
		m.transformVectorConst(dir);
		m.transformVectorConst(start);
		m.translateVector(start);
		return Ray(start, dir);
	}
	
	/*inline void flipZ() {
		transform.xz = -transform.xz;
		transform.yz = -transform.yz;
		transform.zz = -transform.zz;
	}*/
	
	inline bool getOrtho() const {
		return ortho;
	}
	inline void setOrtho(bool o) {
		ortho = o;
	}
	
	inline float getFov() const {
		return fov;
	}
	inline void setFov(float f) {
		fov = f;
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
	float fov;
	
	int resX;
	int resY;
	float aspect;
};
