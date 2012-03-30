#pragma once

#include "matrix.h"
#include "vector.h"
#include "iscene_element.h"

class Light : public ISceneElement {
public:
	Light() {
		//type = 0;
		//radius = 10.0f;
		intensity = 1.0f;
		colour.x = colour.y = colour.z = 1.0f;
	}
	~Light() {}
	
	virtual bool emitsLight() const {
		return true;
	}
	virtual bool isCollidable() const {
		return false;
	}
	
	
	
//protected:
	//int type;
	//float radius;
	float intensity;
	//Vector4f direction;
	Vector4f colour;
};
