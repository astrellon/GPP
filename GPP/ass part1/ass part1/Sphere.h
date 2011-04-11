#pragma once

#include <Vector3.hpp>
#include <Windows.h>
#include <gl/GL.h>

#ifndef isnan
inline bool isnan(double x) {
    return x != x;
}
#endif


typedef struct _Sphere
{
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_forces;
	Vector3 m_acc;

	_Sphere();

	//_Sphere(const Vector3 &vc3Position, const float &fRadius, const float &fMass);
	
	float m_radius;
	float m_mass;

	//float m_red;
	GLuint m_colour;

	void collideWith2(const double &fDt, const _Sphere &sSphere);

	void update(const double &fDt);

	//void refresh();
} Sphere;