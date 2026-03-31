#pragma once

#include <glm/glm.hpp>
#include "../include/Shader.h"

class SpotLight {
public:
    glm::vec3 Position;
    glm::vec3 Direction;

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;

    float CutOff;
    float OuterCutOff;

    float Constant;
    float Linear;
    float Quadratic;

    SpotLight(
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
    );

    void ApplyToShader(Shader& shader, const char* uniformName = "spotLight");
};
