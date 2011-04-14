#include "Sphere.h"

// Creates a panel based sphere mesh.
Mesh createSphere(unsigned int latDivs, unsigned int lonDivs)
{
	// Clamp the latitude and longitude divisions to 3 as the function will not
	// produce useful results otherwise.
	if (latDivs < 3)
		latDivs = 3;

	if (lonDivs < 3)
		lonDivs = 3;

	Mesh mesh;
	// The number of verticies is based off the number of latitude divisions
	// and longitude divisions. The equation is what it is because the top and
	// bottom vertex is reused (the + 2 part).
	unsigned int numVerts = latDivs * (lonDivs - 1) + 2;
	float *verts = (float *)malloc(numVerts * 3 * sizeof(float));

	unsigned int numTriangles = (lonDivs - 1) * latDivs * 2;
	unsigned int numIndicies = numTriangles * 3;

	unsigned int *indices = (unsigned int *)malloc(numIndicies * sizeof(unsigned int));

	unsigned int indexPos = 0;
	unsigned int vertexPos = 0;

	unsigned int lat, lon;
	
	float latStep = (float)(M_PI * 2.0f / (float)latDivs);
	float lonStep = (float)(M_PI / (float)lonDivs);

	mesh.numIndicies = numIndicies;

	// Add top vertex.
	verts[vertexPos] = 0.0f;
	verts[vertexPos + 1] = 1.0f;
	verts[vertexPos + 2] = 0.0f;

	vertexPos += 3;

	// Middle strip verticies.
	for(lon = 1; lon < lonDivs; lon++)
	{
		for(lat = 0; lat < latDivs; lat++)
		{
			float y = sinf(lonStep * lon);
			verts[vertexPos + 1] = cosf(lonStep * lon);
			verts[vertexPos + 0] = y * cosf(latStep * lat);
			verts[vertexPos + 2] = y * sinf(latStep * lat);
						
			vertexPos += 3;
		}
	}

	// Add bottom vertex
	verts[vertexPos] = 0.0f;
	verts[vertexPos + 1] = -1.0f;
	verts[vertexPos + 2] = 0.0f;

	// Add top indices
	for(lat = 1; lat <= latDivs; lat++)
	{
		unsigned int nextIndex = lat + 1;
		if (nextIndex > latDivs)
			nextIndex = 1;

		indices[indexPos] = 0;
		indices[indexPos + 1] = lat;
		indices[indexPos + 2] = nextIndex;
		indexPos += 3;
	}

	// Middle indices strip
	for(lon = 1; lon < lonDivs - 1; lon++)
	{
		unsigned int start = (lon - 1) * latDivs + 1;
		for(lat = start; lat < lon * latDivs + 1; lat++)
		{
			unsigned int nextIndex = lat + 1;
			if (nextIndex >= lon * latDivs + 1)
				nextIndex = start;

			indices[indexPos] = lat;
			indices[indexPos + 1] = lat + latDivs;
			indices[indexPos + 2] = nextIndex;

			indices[indexPos + 3] = lat + latDivs;
			indices[indexPos + 4] = nextIndex + latDivs;
			indices[indexPos + 5] = nextIndex;

			indexPos += 6;
		}
	}

	// Bottom indices
	for(lat = numVerts - latDivs - 1; lat < numVerts - 1; lat++)
	{
		unsigned int nextIndex = lat + 1;
		if (nextIndex >= numVerts - 1)
			nextIndex = numVerts - latDivs - 1;

		indices[indexPos] = numVerts - 1;
		indices[indexPos + 2] = lat;
		indices[indexPos + 1] = nextIndex;

		indexPos += 3;
	}

	mesh.numVerticies = numVerts;

	glGenBuffers(1, &mesh.vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVerts * 3, verts, GL_STATIC_DRAW);
	// Once we have put the vertex data into an array buffer, we don't need to keep
	// a local copy of the verticies.
	free(verts);

	glGenBuffers(1, &mesh.indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndicies, indices, GL_STATIC_DRAW);
	// We also don't need the indices array one it's put into the element array buffer.
	free(indices);

	// Set that each vertex in the array buffer is made up of 3 floats.
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	return mesh;
}