#pragma once

#include "transform.h"

class ISceneElement {
public:
	virtual Transform &getTransform() {
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
		return transform.getPosition();
	}
	
protected:
	Transform transform;
	bool enabled;
};
