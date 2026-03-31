#include "../include/SpotLight.h"
#include <string>

SpotLight::SpotLight(
    glm::vec3 position,
    glm::vec3 direction,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float cutOff,
    float outerCutOff,
    float constant,
    float linear,
    float quadratic
) {
    Position = position;
    Direction = direction;
    Ambient = ambient;
    Diffuse = diffuse;
    Specular = specular;
    CutOff = cutOff;
    OuterCutOff = outerCutOff;
    Constant = constant;
    Linear = linear;
    Quadratic = quadratic;
}

void SpotLight::ApplyToShader(Shader& shader, const char* uniformName) {
    std::string name = uniformName;
    shader.setVec3(name + ".position", Position);
    shader.setVec3(name + ".direction", Direction);
    shader.setVec3(name + ".ambient", Ambient);
    shader.setVec3(name + ".diffuse", Diffuse);
    shader.setVec3(name + ".specular", Specular);
    shader.setFloat(name + ".cutOff", CutOff);
    shader.setFloat(name + ".outerCutOff", OuterCutOff);
    shader.setFloat(name + ".constant", Constant);
    shader.setFloat(name + ".linear", Linear);
    shader.setFloat(name + ".quadratic", Quadratic);
}
