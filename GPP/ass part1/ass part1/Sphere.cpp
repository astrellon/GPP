#include "Sphere.h"

#include <iostream>

using namespace std;

Sphere::_Sphere() : m_mass(1.0f), m_radius(1.0f), m_red(1.0f)
{
}
/*
Sphere::_Sphere(const _Sphere &sSphere) : m_mass(sSphere.m_mass), m_radius(sSphere.m_radius), m_red(sSphere.m_red)
{
	m_position = sSphere.m_position;
	m_velocity = sSphere.m_velocity;
	//m_forces = sSphere.m_forces;

}
*/
Sphere::_Sphere(const Vector3 &vc3Position, const float &fRadius, const float &fMass) : m_radius(fRadius), m_mass(fMass), m_red(0.0f)
{
	m_position = vc3Position;
}

void Sphere::update(const float &fDt)
{
	float roomSize = 10.0f - m_radius;

	if(m_position.m_fY <= -roomSize && m_velocity.m_fY <= 0.0f)
	{
		if(m_velocity.m_fY > -0.01f)
		{
			m_velocity.m_fY = 0.0f;
		}
		else
		{
			m_velocity.m_fY *= -0.6f;
		}
		m_velocity.m_fX *= 0.9f;
		m_velocity.m_fZ *= 0.9f;
		m_position.m_fY = -roomSize;
	}
	else
	{
		//m_forces.m_fY -= 9.8f * m_mass;
	}
	Vector3 acc = m_forces / m_mass;
	m_velocity += fDt * 0.5f * acc;
	m_position += m_velocity * fDt;
	//m_velocity *= 0.975f;

	if(m_position.m_fX < -roomSize && m_velocity.m_fX < 0)
	{
		m_velocity.m_fX *= -0.6f;
		m_position.m_fX = -roomSize;
	}
	else if(m_position.m_fX > roomSize  && m_velocity.m_fX > 0)
	{
		m_velocity.m_fX *= -0.6f;
		m_position.m_fX = roomSize;
	}
	if(m_position.m_fZ < -roomSize && m_velocity.m_fZ < 0)
	{
		m_velocity.m_fZ *= -0.6f;
		m_position.m_fZ = -roomSize;
	}
	else if(m_position.m_fZ > roomSize && m_velocity.m_fZ > 0)
	{
		m_velocity.m_fZ *= -0.6f;
		m_position.m_fZ = roomSize;
	}
	m_forces.m_fX = m_forces.m_fY = m_forces.m_fZ = 0.0f;
}

void Sphere::collideWith(const float &fDt, Sphere &sSphere)
{
	Vector3 toCenter = m_position - sSphere.m_position;
	float x = length(toCenter) - m_radius - sSphere.m_radius;
	if(x < 0)
	{
		Vector3 d = normalize(toCenter);
		float dot = dot3(normalize(sSphere.m_velocity), d);
		Vector3 vs = sSphere.m_velocity * dot / fDt;

		//m_temp_velo += 100 * vs * sSphere.m_mass;
		m_forces += /*-500.0f * x * toCenter */ 3 * vs * sSphere.m_mass;

		vs = m_velocity * dot3(normalize(m_velocity), d) / fDt;
		//sSphere.m_temp_velo += -100 * vs * m_mass;
		sSphere.m_forces -= /*-500.0f * x * toCenter*/ 3 * vs * m_mass;
	}
}