#include "../include/Rover.h"
#include <GLFW/glfw3.h>

Rover::Rover(VertexObject* inStructureVAO, VertexObject* cylVAO, VertexObject* torVAO, float tireRadius, float tireWidth, float numberOfSpokes)
    : tire(cylVAO, torVAO, tireRadius, tireWidth, numberOfSpokes), structureVAO(inStructureVAO)
{
    chassisSize = glm::vec3(2.0f, 0.45f, 3.0f);
    position = glm::vec3(0.0f, 0.0f, -5.0f); // Default starting position
    wheelAngle = 0.0f;
    speed = 5.0f;
}


void Rover::Update(GLFWwindow* window, float deltaTime) {
    // Determine movement distance for this frame based on speed and time
    float moveDistance = speed * deltaTime;
    bool moved = false;

    // Using Arrow Keys (UP/DOWN for Z-axis, LEFT/RIGHT for X-axis) to drive the Rover
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position.z -= moveDistance;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position.z += moveDistance;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position.x -= moveDistance;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position.x += moveDistance;
        moved = true;
    }

    if (moved) {
        // Calculate physics-based tire rotation based on arc length formula 
        // distanceMoved = r * theta => theta = distanceMoved / r
        // For simplicity, using moveDistance against tire radius to incrementally rotate
        wheelAngle += moveDistance / tire.tireRadius;
    }
}


void Rover::Draw(Shader& shader, glm::mat4 parentTransform) {
    // Draw a simple support-arm assembly for each wheel.
    // This replaces the old direct body-to-wheel attachment with:
    // rover root -> support anchor -> vertical hanger -> horizontal arm -> wheel mount -> wheel
    float sideAnchorX = chassisSize.x * 0.33f;
    float wheelCenterX = chassisSize.x * 0.56f + tire.tireWidth;
    float zOffset = (chassisSize.z / 2.0f) - tire.tireRadius - 0.1f;
    float topSupportY = tire.tireRadius + 0.52f;
    float wheelCenterY = tire.tireRadius;
    float verticalHeight = topSupportY - wheelCenterY;
    float horizontalLength = wheelCenterX - sideAnchorX;

    glm::vec3 supportColor(0.33f, 0.28f, 0.22f);
    glm::vec3 mountColor(0.42f, 0.42f, 0.42f);

    auto drawSupportCube = [&](const glm::mat4& localTransform, const glm::vec3& color) {
        if (structureVAO == nullptr) {
            return;
        }

        glBindVertexArray(structureVAO->getVAO());
        shader.setMat4("model", parentTransform * localTransform);
        shader.setVec3("objectColor", color);
        glDrawElements(GL_TRIANGLES, structureVAO->getVertexCount(), GL_UNSIGNED_INT, 0);
    };

    glm::vec3 tirePositions[4] = {
        glm::vec3(wheelCenterX, wheelCenterY,  zOffset),  // Front-Right
        glm::vec3(-wheelCenterX, wheelCenterY,  zOffset), // Front-Left
        glm::vec3(wheelCenterX, wheelCenterY, -zOffset),  // Back-Right
        glm::vec3(-wheelCenterX, wheelCenterY, -zOffset)  // Back-Left
    };

    for (int i = 0; i < 4; i++) {
        float sideSign = (tirePositions[i].x >= 0.0f) ? 1.0f : -1.0f;
        glm::vec3 supportAnchor(sideSign * sideAnchorX, topSupportY, tirePositions[i].z);

        glm::mat4 verticalLocal = glm::mat4(1.0f);
        verticalLocal = glm::translate(
            verticalLocal,
            glm::vec3(supportAnchor.x, wheelCenterY + verticalHeight * 0.5f, supportAnchor.z)
        );
        verticalLocal = glm::scale(verticalLocal, glm::vec3(0.10f, verticalHeight, 0.16f));
        drawSupportCube(verticalLocal, supportColor);

        glm::mat4 horizontalLocal = glm::mat4(1.0f);
        horizontalLocal = glm::translate(
            horizontalLocal,
            glm::vec3(sideSign * (sideAnchorX + horizontalLength * 0.5f), wheelCenterY + 0.05f, supportAnchor.z)
        );
        horizontalLocal = glm::scale(horizontalLocal, glm::vec3(horizontalLength, 0.10f, 0.14f));
        drawSupportCube(horizontalLocal, supportColor);

        glm::mat4 mountLocal = glm::mat4(1.0f);
        mountLocal = glm::translate(
            mountLocal,
            glm::vec3(tirePositions[i].x - sideSign * (tire.tireWidth * 0.22f), tirePositions[i].y, tirePositions[i].z)
        );
        mountLocal = glm::scale(mountLocal, glm::vec3(0.12f, 0.18f, 0.18f));
        drawSupportCube(mountLocal, mountColor);

        glm::mat4 tireLocal = glm::mat4(1.0f);

        // 1. Move to the end of the support assembly
        tireLocal = glm::translate(tireLocal, tirePositions[i]);

        // 2. Stand the tire up (Native cylinder is flat on Y. Rotate 90 deg around Z to make axle align with X)
        tireLocal = glm::rotate(tireLocal, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // 3. Roll the wheel around its axle
        tireLocal = glm::rotate(tireLocal, -wheelAngle, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 tireWorld = parentTransform * tireLocal;
        tire.Draw(shader, tireWorld);
    }

}
