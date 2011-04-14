#pragma once

#include "Vector3.hpp"
#include <Windows.h>
#include "GLee\GLee.h"
#include <gl\GL.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

typedef struct
{
	GLuint numIndicies;
	GLuint numVerticies;
	GLuint vertexBufferId;
	GLuint indexBufferId;
} Mesh;

Mesh createSphere(unsigned int latDivs, unsigned int lonDivs);
