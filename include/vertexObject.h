#pragma once

// Create a class of vertexObject
class VertexObject {
private:
	unsigned int VAO = 0; 
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	unsigned int vertexCount = 0;
public:
	void setVertexCount(unsigned int count) {
		vertexCount = count;
	}
	unsigned int& getVAO() {
		return VAO;
	}
	unsigned int& getVBO() {
		return VBO;
	}
	unsigned int& getEBO() {
		return EBO;
	}
	unsigned int getVertexCount() {
		return vertexCount;
	}
};

VertexObject createVAOWithPositionAndNormal(std::vector<float>vertices, std::vector<unsigned int>indices);
