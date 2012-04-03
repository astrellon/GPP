#pragma once

#include "vector.h"
//#include "icollidable.h"

class IRenderable;

class RayHitResult {
public:
	RayHitResult() : target(NULL) {}
	~RayHitResult() {}

	const IRenderable *target;
	Vector4f worldPos;
	Vector4f normal;
	float len;
};
