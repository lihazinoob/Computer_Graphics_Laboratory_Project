#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/Shader.h"
#include "../include/vertexObject.h"
#include "../include/Tire.h"

struct GLFWwindow;

class Rover {
public:
    Tire tire;

    VertexObject* structureVAO;
    VertexObject* coneVAO;

    // Dimensions
    glm::vec3 chassisSize;
    
    //Position
    glm::vec3 position;

    
    float wheelAngle;      // Tracks how much the wheels have spun
    float speed;           // Current movement speed

    // Constructor gets the required geometries
    Rover(
        VertexObject* structureVAO,
        VertexObject* coneVAO,
        VertexObject* cylVAO,
        VertexObject* torVAO,
        float tireRadius,
        float tireWidth,
        float numberOfSpokes
    );

    // Update Rover based on input
    void Update(GLFWwindow* window, float deltaTime);


    // Draws the entire rover assembly based on a single parent transform
    void Draw(Shader& shader, glm::mat4 parentTransform);
};
