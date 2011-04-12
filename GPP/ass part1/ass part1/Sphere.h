#pragma once

#include <Vector3.hpp>
#include <Windows.h>
#include <gl/GL.h>

#include "Globals.h"

#ifndef isnan
inline bool isnan(double x) {
    return x != x;
}
#endif


typedef struct _Sphere
{
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_acc;

	_Sphere();

	//_Sphere(const Vector3 &vc3Position, const float &fRadius, const float &fMass);
	
	float m_radius;
	float m_mass;

	//float m_red;
	GLuint m_colour;
	/*
	FORCELINE void calcForce(Vector3 &forces, const float &springConst, const float &dampFactor, const float &overlap, const Vector3 &tangent, const Vector3 &newVelocity)
	{
		Vector3 vs = tangent * dot3(tangent, newVelocity);
		forces -= (springConst * overlap * tangent) + (dampFactor * vs);
	}*/
	/*
	void update(const double &fDt);

	void update2(const double &fDt);
	*/
	//void refresh();
} Sphere;