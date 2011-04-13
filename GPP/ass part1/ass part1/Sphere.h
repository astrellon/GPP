#pragma once

#include <Vector3.hpp>

typedef struct _Sphere
{
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_acc;

	_Sphere();

	float m_radius;
	float m_mass;

	unsigned int m_colour;

} Sphere;