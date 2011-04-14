// CSE4GPP Assignment part 1 2011
// Alan Lawrey 15547299

#pragma once

#include "Vector3.hpp"
#include <Windows.h>
#include "GLee\GLee.h"
#include <gl\GL.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

// Simple sphere struct that contains all the data of a sphere except
// for its position which is kept in another Vector3 array.
typedef struct
{
	Vector3 m_velocity;
	Vector3 m_acc;
} SphereData;

// Simple mesh struct which contains the data required to call a
// glDrawElements/glDrawElementsInstanced on the data.
typedef struct
{
	GLuint numIndicies;
	GLuint numVerticies;
	GLuint vertexBufferId;
	GLuint indexBufferId;
} Mesh;

// Creates a sphere with the given latitude and longitude divisions.
// Divisions less than 3 are clamped.
// The radius of the returned mesh will be 1.0f, which makes its diameter 2.0f.
Mesh createSphere(unsigned int latDivs, unsigned int lonDivs);
