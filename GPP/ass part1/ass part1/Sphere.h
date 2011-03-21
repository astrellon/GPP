#pragma once

#include <Vector3.hpp>

typedef struct _Sphere
{
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_forces;
	//Vector3 m_temp_velo;

	_Sphere();

	//_Sphere(const _Sphere &sSphere);

	_Sphere(const Vector3 &vc3Position, const float &fRadius, const float &fMass);
	
	float m_radius;
	float m_mass;

	float m_red;
	float pad2;

	void collideWith(const float &fDt, _Sphere &sSphere);

	void update(const float &fDt);

	//void refresh();
} Sphere;