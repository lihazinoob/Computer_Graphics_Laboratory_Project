#pragma once
#include<glm/glm.hpp>
#include "../include/Shader.h"

class DirectionalLight {
public:
	glm::vec3 Direction;
	
	
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void ApplyToShader(Shader& shader, const char* uniformName = "dirLight");
};