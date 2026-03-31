
#include<vector>
#include "../include/vertexObject.h"
#include<glad/glad.h>

VertexObject createVAOWithPositionAndNormal(std::vector<float>vertices,std::vector<unsigned int>indices) {
	// Create an instance of VertexObject
	VertexObject vertexObject;

	// Calculate the number of vertices from the indices vector and set to the VertexObject
	vertexObject.setVertexCount(indices.size());
	// Generate 1 vertex array object in OpenGL and stores its unique id to the memory by vertexObject.VAO
	glGenVertexArrays(1, &vertexObject.getVAO());
	// Generate 1 vertex buffer object in OpenGL and stores its unique id to the memory by vertexObject.VBO
	glGenBuffers(1, &vertexObject.getVBO());
	// Generate 1 Element Buffer object in OpenGL and stores its unique id to the memory by vertexObject.EBO
	glGenBuffers(1, &vertexObject.getEBO());

	// Bind the VAO
	glBindVertexArray(vertexObject.getVAO());

	// Bind and Populate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexObject.getVBO());
	int verticesSize = vertices.size() * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), GL_STATIC_DRAW);

	// Bind and Populate the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexObject.getEBO());
	int indicesSize = indices.size() * sizeof(unsigned int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);

	// Position Attribute (location = 0, 3 floats, stride = 6 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal Attribute (location = 1, 3 floats, stride = 6 floats, offset = 3 floats)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VBO and VAO safely (Do NOT unbind EBO while VAO is active)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexObject;

}

VertexObject createVAOWithPositionNormalAndTexCoord(std::vector<float> vertices, std::vector<unsigned int> indices) {
	VertexObject vertexObject;

	vertexObject.setVertexCount(indices.size());
	glGenVertexArrays(1, &vertexObject.getVAO());
	glGenBuffers(1, &vertexObject.getVBO());
	glGenBuffers(1, &vertexObject.getEBO());

	glBindVertexArray(vertexObject.getVAO());

	glBindBuffer(GL_ARRAY_BUFFER, vertexObject.getVBO());
	int verticesSize = vertices.size() * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexObject.getEBO());
	int indicesSize = indices.size() * sizeof(unsigned int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexObject;
}
