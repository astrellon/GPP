#pragma once

#include "material.h"
#include "iscene_element.h"
#include "ray.h"
#include "ray_hit_result.h"

class IRenderable : public ISceneElement {
public:
	const Material *getMaterial() const {
		return material;
	}
	Material *getMaterial() {
		return material;
	}
	void setMaterial(Material *mat) {
		material = mat;
	}
	virtual bool isCollidable() const {
		return true;
	}
	
	virtual bool collides(const Ray &r, RayHitResult &result) const = 0;
protected:
	Material *material;
};
