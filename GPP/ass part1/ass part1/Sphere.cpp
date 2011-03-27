#include "Sphere.h"

#include <iostream>

using namespace std;

#define BOUNCE 0.6f

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
	m_forces.m_fX = m_forces.m_fY = m_forces.m_fZ = 0.0f;
}

void Sphere::collideWith(const float &fDt, Sphere &sSphere)
{
	Vector3 toCenter = m_position - sSphere.m_position;
	float x = length(toCenter) - m_radius - sSphere.m_radius;
	if(x < 0)
	{
		Vector3 d = normalize(toCenter);
		float dot1 = dot3(normalize(sSphere.m_velocity), d);
		if(isnan(dot1))
			dot1 = 0;
		float dot2 = dot3(normalize(m_velocity), d);
		if(isnan(dot2))
			dot2 = 0;
		//if(dot1 < 0 && dot2 > 0)
		//	return;

		//Vector3 vs = sSphere.m_velocity * dot1;// / fDt * 0.5f;

		//m_temp_velo += 100 * vs * sSphere.m_mass;
		m_forces += -20.0f * x * toCenter;// + 10.24f * vs * sSphere.m_mass;

		//vs = m_velocity * dot2;// / fDt * 0.5f;
		//sSphere.m_temp_velo += -100 * vs * m_mass;
		sSphere.m_forces -= -20.0f * x * toCenter;// + 10.24f * vs * m_mass;
	}
}

void Sphere::collideWith2(const float &fDt, const _Sphere &sSphere)
{
	Vector3 toCentre = (m_position + m_velocity * fDt) - (sSphere.m_position + sSphere.m_velocity * fDt);
	//Vector3 toCentre = (sSphere.m_position) - (m_position);
	float lenSqrd = lengthSquared(toCentre);
	float len = sqrt(lenSqrd);
	float x = len - m_radius - sSphere.m_radius;
	if(x < 0)
	{
		//Vector3 vs = dot3(toCentre, sSphere.m_velocity) / lengthSquared(toCentre) * toCentre;
		//float len2 = length(sSphere.m_velocity);
		//float angle = dot3(toCentre, sSphere.m_velocity);
		
		//Vector3 vs = angle / lengthSquared(sSphere.m_velocity) * sSphere.m_velocity;
		//m_forces += /*-20.0f * x * toCentre / len + */sSphere.m_mass * 0.6f * vs;// / fDt;

		float resp = 1.0f / lenSqrd;

		float dot1 = dot3(m_velocity, toCentre) * resp;
		float dot2 = dot3(sSphere.m_velocity, toCentre) * resp;
		if(dot1 < 0 && dot2 > 0)
		{
			Vector3 vs1 = dot1 * toCentre;
			Vector3 vs2 = dot2 * toCentre;
			m_forces += ((vs2 * sSphere.m_mass) - (vs1 * m_mass)) * 0.5f / fDt;
		}
		m_forces -= 80.0f * x * (toCentre / len);
	}
}