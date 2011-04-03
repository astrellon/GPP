#include "Sphere.h"

#include <iostream>

using namespace std;

#define BOUNCE 0.6f

Sphere::_Sphere() : m_mass(1.0f), m_radius(1.0f), m_red(1.0f)
{
}

Sphere::_Sphere(const Vector3 &vc3Position, const float &fRadius, const float &fMass) : m_radius(fRadius), m_mass(fMass), m_red(0.0f)
{
	m_position = vc3Position;
}

/*void Sphere::update(const float &fDt)
{
	float roomSize = 20.0f - m_radius;

	if(m_velocity.m_fY <= 0.0f && m_position.m_fY <= -roomSize)
	{
		if(m_acc.m_fY > -10)
		{
			m_velocity.m_fY = 0.0f;
		}
		else
		{
			m_velocity.m_fY *= -BOUNCE;
		}
		m_velocity.m_fX *= 0.9f;
		m_velocity.m_fZ *= 0.9f;
		m_position.m_fY = -roomSize;
	}
	else
	{
		m_forces.m_fY -= 9.8f * m_mass;// * fDt;
	}

	Vector3 halfVelo = m_velocity + fDt * 0.5f * m_acc;
	m_position += halfVelo * fDt;
	m_acc = m_forces / m_mass;
	m_velocity = halfVelo + 0.5f * m_acc * fDt;

	//m_velocity *= 0.975f;

	//cout << m_velocity.m_fX << ", " << m_velocity.m_fY << ", " << m_velocity.m_fZ << endl;

	if(m_position.m_fX < -roomSize && m_velocity.m_fX < 0)
	{
		m_velocity.m_fX *= -BOUNCE;
		m_position.m_fX = -roomSize;
	}
	else if(m_position.m_fX > roomSize  && m_velocity.m_fX > 0)
	{
		m_velocity.m_fX *= -BOUNCE;
		m_position.m_fX = roomSize;
	}
	if(m_position.m_fZ < -roomSize && m_velocity.m_fZ < 0)
	{
		m_velocity.m_fZ *= -BOUNCE;
		m_position.m_fZ = -roomSize;
	}
	else if(m_position.m_fZ > roomSize && m_velocity.m_fZ > 0)
	{
		m_velocity.m_fZ *= -BOUNCE;
		m_position.m_fZ = roomSize;
	}
	//m_forces.m_fX = m_forces.m_fY = m_forces.m_fZ = 0.0f;
	m_forces = Vector3(0.0f);
}*/

void Sphere::update(const double &fDt)
{
	float roomSize = 20.0f - m_radius;

	if(m_velocity.getY() <= 0.0f && m_position.getY() <= -roomSize)
	{
		if(m_acc.getY() > -10)
		{
			m_velocity.setY(0.0f);
		}
		else
		{
			m_velocity.setY(m_velocity.getY() * -BOUNCE);
		}
		m_velocity.setX(m_velocity.getX() * 0.9f);
		m_velocity.setZ(m_velocity.getZ() * 0.9f);
		m_position.setY(-roomSize);
	}
	else
	{
		m_forces.setY(m_forces.getY() - 9.8f * m_mass);// * fDt;
	}

	Vector3 halfVelo = m_velocity + fDt * 0.5f * m_acc;
	m_position += halfVelo * fDt;
	m_acc = m_forces / m_mass;
	m_velocity = halfVelo + 0.5f * m_acc * fDt;
	/*
#ifdef _SSE
	float position[4];
	float velocity[4];

	_mm_store_ps(position, m_position.m_v128);
	_mm_store_ps(velocity, m_velocity.m_v128);
#else
	float *position;
	float *velocity;

	position = (float*)&m_position.m_fX;
	velocity = (float*)&m_velocity.m_fX;
#endif

	if(position[0] < -roomSize && velocity[0] < 0)
	{
		velocity[0] *= -BOUNCE;
		position[0] = -roomSize;
	}
	else if(position[0] > roomSize && velocity[0] > 0)
	{
		velocity[0] *= -BOUNCE;
		position[0] = roomSize;
	}
	if(position[2] < -roomSize && velocity[2] < 0)
	{
		velocity[2] *= -BOUNCE;
		position[2] = -roomSize;
	}
	else if(position[2] > roomSize && velocity[2] > 0)
	{
		velocity[2] *= -BOUNCE;
		position[2] = roomSize;
	}

#ifdef _SSE
	m_position.m_v128 = _mm_load_ps(position);
	m_velocity.m_v128 = _mm_load_ps(velocity);
#endif*/

	m_forces = Vector3(0.0f);
}

void Sphere::collideWith2(const double &fDt, const _Sphere &sSphere)
{
	Vector3 toCentre = (m_position + m_velocity * fDt) - (sSphere.m_position + sSphere.m_velocity * fDt);
	//Vector3 toCentre = (sSphere.m_position) - (m_position);
	float lenSqrd = lengthSquared(toCentre);
	float len = sqrt(lenSqrd);
	float x = len - m_radius - sSphere.m_radius;
	if(x < 0)
	{
		float resp = 1.0f / lenSqrd;

		float dot1 = dot3(m_velocity, toCentre) * resp;
		float dot2 = dot3(sSphere.m_velocity, toCentre) * resp;
		if(dot1 < 0 && dot2 > 0)
		{
			Vector3 vs1 = dot1 * toCentre;
			Vector3 vs2 = dot2 * toCentre;
			m_forces += ((vs2 * sSphere.m_mass) - (vs1 * m_mass)) * 0.7f / fDt;
		}
		m_forces -= 80.0f * x * (toCentre / len);
	}
}