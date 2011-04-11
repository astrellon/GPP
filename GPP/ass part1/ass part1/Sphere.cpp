#include "Sphere.h"

#include <iostream>

using namespace std;

#define BOUNCE 0.6f

//Sphere::_Sphere() : m_position(0), m_mass(1.0f), m_radius(1.0f), m_colour(0), m_velocity(0), m_acc(0), m_forces(0)
Sphere::_Sphere() : m_mass(1.0f), m_radius(1.0f), m_colour(0)
{
	m_acc = Vector3(0);
	m_forces = Vector3(0);
}
/*
Sphere::_Sphere(const Vector3 &vc3Position, const float &fRadius, const float &fMass) : m_radius(fRadius), m_mass(fMass), m_colour(0), m_velocity(0), m_acc(0), m_forces(0)
{
	m_position = vc3Position;
}
*/
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
	float roomSize = ROOM_SIZE - m_radius;

	const Vector3 bottom(0, 1, 0);
	const Vector3 left(-1, 0, 0);
	const Vector3 right(1, 0, 0);
	const Vector3 front(0, 0, -1);
	const Vector3 back(0, 0, 1);

	Vector3 halfVelo = m_velocity + fDt * 0.5f * m_acc;
	m_position += halfVelo * fDt;
	m_acc = m_forces / m_mass;
	m_velocity = halfVelo + 0.5f * m_acc * fDt;

	//m_forces.setY(m_forces.getY() - 9.8f * m_mass);// * fDt;
	m_velocity -= Vector3(0.0f, 9.8f * fDt, 0.0f);

	float distance = dot3(m_position, bottom) + roomSize;
	if(distance < 0 && dot3(m_velocity, bottom) < 0)
	{
		m_velocity -= 1.5 * bottom * dot3(m_velocity, bottom);
		m_position.setY(-roomSize);
	}

	distance = dot3(m_position, left) + roomSize;
	if(distance < 0 && dot3(m_velocity, left) < 0)
	{
		m_velocity -= 1.5 * left * dot3(m_velocity, left);
		m_position.setX(roomSize);
	}

	distance = dot3(m_position, right) + roomSize;
	if(distance < 0 && dot3(m_velocity, right) < 0)
	{
		m_velocity -= 1.5 * right * dot3(m_velocity, right);
		m_position.setX(-roomSize);
	}

	distance = dot3(m_position, front) + roomSize;
	if(distance < 0 && dot3(m_velocity, front) < 0)
	{
		m_velocity -= 1.5 * front * dot3(m_velocity, front);
		m_position.setZ(roomSize);
	}

	distance = dot3(m_position, back) + roomSize;
	if(distance < 0 && dot3(m_velocity, back) < 0)
	{
		m_velocity -= 1.5 * back * dot3(m_velocity, back);
		m_position.setZ(-roomSize);
	}


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
	if(lenSqrd == 0)
		return;
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
		m_forces -= 120.0f * x * (toCentre / len);
	}
}

void Sphere::collideWith3(const double &fDt, const _Sphere &sSphere)
{
	Vector3 toCentre = m_position - sSphere.m_position;
	//toCentre = normalize(toCentre);

	float relNv = dot3((sSphere.m_velocity - m_velocity), toCentre);

	float dist = length(toCentre) - m_radius - sSphere.m_radius;

	float remove = relNv + 0.4* (dist + 1) / fDt;
	if(remove < 0 && dist < 0)
	{
		float imp = remove / (m_mass + sSphere.m_mass);

		//m_velocity += imp * toCentre * m_mass;
		m_forces -= imp * toCentre * m_mass / fDt * 0.05f;
	}
}