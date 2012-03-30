#pragma once

#include "matrix.h"

class ISceneElement {
public:
	virtual Matrix4f &getTransform() {
		return transform;
	}
	
	virtual bool emitsLight() const = 0;
	virtual bool isCollidable() const = 0;
	
	virtual bool isEnabled() const {
		return enabled;
	}
	virtual void setEnabled(bool e) {
		enabled = e;
	}
	Vector4f getPosition() const {
		return Vector4f(transform.wx, transform.wy, transform.wz);
	}
	
protected:
	Matrix4f transform;
	bool enabled;
};
