#include "vertexObject.h"

VertexObject createVAO(const float* vertices, unsigned int vSize, const unsigned int* indices, unsigned int iSize) {

	VertexObject vo;
	vo.vertexCount = iSize / sizeof(unsigned int);


	// create 1 vertex array object and obtain the id of this VAO
	glGenVertexArrays(1, &vo.VAO);
	// create 1 vertex buffer object and obtain the id of this VBO
	glGenBuffers(1, &vo.VBO);
	// create 1 element buffer object and obtain the id of this EBO
	glGenBuffers(1, &vo.EBO);

	// bind the Vertex Array Object with the Vertex Buffer Object
	glBindVertexArray(vo.VAO);

	// allocates the data with the size of linevertices in GPU from CPU
	glBindBuffer(GL_ARRAY_BUFFER, vo.VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vo.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);

	// define the vertex attribute (for position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// define the vertex attribute (for color)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return vo;
}
