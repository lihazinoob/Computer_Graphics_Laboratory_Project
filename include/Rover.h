#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/Shader.h"
#include "../include/vertexObject.h"
#include "../include/Tire.h"

class Rover {
public:
    Tire tire;

    // Dimensions
    glm::vec3 chassisSize;


    // Constructor gets the required geometries
    Rover(VertexObject* cylVAO, VertexObject* torVAO,float tireRadius,float tireWidth,float numberOfSpokes);

    // Draws the entire rover assembly based on a single parent transform
    void Draw(Shader& shader, glm::mat4 parentTransform);
};