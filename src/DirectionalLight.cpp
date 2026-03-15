#include "../include/DirectionalLight.h"
#include "../include/Shader.h"
#include <string>

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	Direction = direction;
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
}

void DirectionalLight::ApplyToShader(Shader& shader, const char* uniformName) {
	std::string name = uniformName;
	shader.setVec3(name + ".direction", Direction);
	shader.setVec3(name + ".ambient", Ambient);
	shader.setVec3(name + ".diffuse", Diffuse);
	shader.setVec3(name + ".specular", Specular);
}

