#pragma once

// Create a class of vertexObject
class VertexObject {
private:
	unsigned int VAO = 0; 
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	int vertexCount = 0;
public:
	void setVertexCount(int count) {
		vertexCount = count;
	}
};