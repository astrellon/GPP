#pragma once

#include "vector.h"
#include "matrix.h"

class Transform {
public:
	Transform() {
		up = REF_UP;
		right = REF_RIGHT;
		forward = REF_FORWARD;
		dirty = false;
	}
	~Transform() {}
	
	void calcMatrix() {
		worldToObj.xx = right.x;
		worldToObj.yx = right.y;
		worldToObj.zx = right.z;
		
		worldToObj.xy = up.x;
		worldToObj.yy = up.y;
		worldToObj.zy = up.z;
		
		/*if (forCamera) {
			worldToObj.xz = -forward.x;
			worldToObj.yz = -forward.y;
			worldToObj.zz = -forward.z;
		}
		else {*/
			worldToObj.xz = forward.x;
			worldToObj.yz = forward.y;
			worldToObj.zz = forward.z;
		//}
		
		worldToObj.wx = position.x;
		worldToObj.wy = position.y;
		worldToObj.wz = position.z;
		worldToObj.ww = 1.0f;
		dirty = false;
	}
	
	Vector4f getUp() const {
		return up;
	}
	void setUp(const Vector4f &val) {
		up = val;
		up.normalise();
		forward = up.cross(right);
		right = forward.cross(up);
		dirty = true;
	}
	
	Vector4f getRight() const {
		return right;
	}
	void setRight(const Vector4f &val) {
		right = val;
		right.normalise();
		forward = up.cross(right);
		up = right.cross(forward);
		dirty = true;
	}
	
	Vector4f getForward() const {
		return forward;
	}
	void setForward(const Vector4f &val) {
		forward = val;
		forward.normalise();
		right = forward.cross(up);
		up = right.cross(forward);
		dirty = true;
	}
	
	Vector4f getPosition() const {
		return position;
	}
	void setPosition(const Vector4f &val) {
		position = val;
		lookAtTarget();
		dirty = true;
	}
	
	Vector4f getTarget() const {
		return target;
	}
	void setTarget(const Vector4f &val) {
		target = val;
		lookAtTarget();
		dirty = true;
	}
	
	void lookAtTarget() {
		Vector4f toTarget = target.sub(position);
		setForward(toTarget);
	}
	void updateTarget() {
		Vector4f toTarget = target.sub(position);
		double len = toTarget.length();
		
		target = position.add(forward.scale(len));
	}
	
	Matrix4f getWorldToObj() const {
		return worldToObj;
	}
	
	Matrix4f &getWorldToObj() {
		preproc();
		return worldToObj;
	}
	void preproc() {
		if (dirty) {
			calcMatrix();
		}
	}
	
	void translate(const Vector4f &rhs) {
		position.addTo(rhs);
		dirty = true;
	}
	void translate(const float x, const float y, const float z) {
		position.addTo(x, y, z);
		dirty = true;
	}
	void rotate(float dx, float dy) {
		Matrix4f m;
		m.rotate(right, dy);
		m.transformVectorConst(forward);
		m.transformVectorConst(up);
		
		if (forCamera) {
			m.rotate(REF_UP, dx);
		}
		else {
			m.rotate(up, dx);
		}
		
		m.transformVectorConst(right);
		m.transformVectorConst(forward);
		
		updateTarget();
		dirty = true;
	}
	
	void orbit(float dx, float dy) {
		Vector4f toPosition = position.sub(target);
		double len = toPosition.length();
		toPosition.scaleBy(1.0 / len);
		Matrix4f m;
		m.rotate(right, dy);
		m.transformVectorConst(toPosition);
		
		if (forCamera) {
			m.rotate(REF_UP, dx);
		}
		else {
			m.rotate(up, dx);
		}
		m.transformVectorConst(toPosition);
		position = target.add(toPosition.scale(len));
		forward = toPosition.scale(-1);
		up = right.cross(forward);
		right = forward.cross(up);
		dirty = true;
	}
	
	void setForCamera(bool forCam) {
		forCamera = forCam;
	}
	bool isForCamera() const {
		return forCamera;
	}
	
	static const Vector4f REF_UP;
	static const Vector4f REF_RIGHT;
	static const Vector4f REF_FORWARD;
	
private:
	bool forCamera;
	bool dirty;
	Vector4f up;
	Vector4f right;
	Vector4f forward;
	Vector4f position;
	Vector4f target;
	Matrix4f worldToObj;
};

const Vector4f Transform::REF_UP(0, 1, 0);
const Vector4f Transform::REF_RIGHT(1, 0, 0);
const Vector4f Transform::REF_FORWARD(0, 0, 1);
