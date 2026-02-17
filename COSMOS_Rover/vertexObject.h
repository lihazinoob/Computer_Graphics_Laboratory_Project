#pragma once
#include <glad/glad.h>

// a Struct to create the Vertex Object
struct VertexObject
{
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	int vertexCount = 0;
};



// Creates a VAO for a simple 3D-position-only line
// Returns VAO ID
VertexObject createVAO(const float* vertices, unsigned int vSize, const unsigned int* indices, unsigned int iSize);

