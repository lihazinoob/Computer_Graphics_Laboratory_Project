#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/Shader.h"
#include "../include/vertexObject.h"

class Tire {
public:
    // References to the base geometries (we pass these in so we don't duplicate VBOs in memory)
    VertexObject* cylinderVAO;

    // Dimensions
    float tireRadius;
    float tireWidth;
    int numSpokes;

    // Colors
    glm::vec3 rubberColor;
    glm::vec3 rimColor;

    Tire(VertexObject* sharedCylinder, float radius = 0.5f, float width = 0.3f, int spokes = 6);

    // Draw the tire given a parent transform matrix
    void Draw(Shader& shader, glm::mat4 parentTransform);
};