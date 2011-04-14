#include "Sphere.h"

Mesh createSphere(unsigned int latDivs, unsigned int lonDivs)
{
	Mesh mesh;
	unsigned int numVerts = latDivs * (lonDivs - 1) + 2;
	float *verts = (float *)malloc(numVerts * 3 * sizeof(float));

	unsigned int numFaces = (lonDivs - 1) * latDivs * 2;
	unsigned int numIndicies = numFaces * 3;

	unsigned int *indicies = (unsigned int *)malloc(numIndicies * sizeof(unsigned int));

	unsigned int indexPos = 0;
	unsigned int vertexPos = 0;

	unsigned int lat, lon;
	
	float latStep = M_PI * 2.0f / (float)latDivs;
	float lonStep = M_PI / (float)lonDivs;

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

	// Add top indicies
	for(lat = 1; lat <= latDivs; lat++)
	{
		int nextIndex = lat + 1;
		if (nextIndex > latDivs)
			nextIndex = 1;

		indicies[indexPos] = 0;
		indicies[indexPos + 1] = lat;
		indicies[indexPos + 2] = nextIndex;
		indexPos += 3;
	}

	// Middle indicies strip
	for(lon = 1; lon < lonDivs - 1; lon++)
	{
		int start = (lon - 1) * latDivs + 1;
		for(lat = start; lat < lon * latDivs + 1; lat++)
		{
			int nextIndex = lat + 1;
			if (nextIndex >= lon * latDivs + 1)
				nextIndex = start;

			indicies[indexPos] = lat;
			indicies[indexPos + 1] = lat + latDivs;
			indicies[indexPos + 2] = nextIndex;

			indicies[indexPos + 3] = lat + latDivs;
			indicies[indexPos + 4] = nextIndex + latDivs;
			indicies[indexPos + 5] = nextIndex;

			indexPos += 6;
		}
	}

	// Bottom indicies
	for(lat = numVerts - latDivs - 1; lat <= numVerts - 1; lat++)
	{
		int nextIndex = lat + 1;
		if (nextIndex >= numVerts - 1)
			nextIndex = numVerts - latDivs - 1;

		indicies[indexPos] = numVerts - 1;
		indicies[indexPos + 2] = lat;
		indicies[indexPos + 1] = nextIndex;

		indexPos += 3;
	}

	mesh.numVerticies = numVerts;

	glGenBuffers(1, &mesh.vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVerts * 3, verts, GL_STATIC_DRAW);
	free(verts);

	glGenBuffers(1, &mesh.indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndicies, indicies, GL_STATIC_DRAW);
	//free(indicies);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	return mesh;
}