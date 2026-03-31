#include "../include/Rover.h"
#include <GLFW/glfw3.h>

Rover::Rover(
    VertexObject* inStructureVAO,
    VertexObject* inConeVAO,
    VertexObject* cylVAO,
    VertexObject* torVAO,
    float tireRadius,
    float tireWidth,
    float numberOfSpokes
)
    : tire(cylVAO, torVAO, tireRadius, tireWidth, numberOfSpokes), structureVAO(inStructureVAO), coneVAO(inConeVAO)
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
    // Build the next rover layer above the wheel supports.
    // Current hierarchy:
    // rover root -> top deck -> lower deck frame -> support assemblies -> wheel mounts -> tires
    float sideAnchorX = chassisSize.x * 0.33f;
    float wheelCenterX = chassisSize.x * 0.56f + tire.tireWidth;
    float zOffset = (chassisSize.z / 2.0f) - tire.tireRadius - 0.1f;
    float topSupportY = tire.tireRadius + 0.52f;
    float wheelCenterY = tire.tireRadius;
    float verticalHeight = topSupportY - wheelCenterY;
    float horizontalLength = wheelCenterX - sideAnchorX;
    float lowerDeckThickness = 0.12f;
    float lowerDeckY = topSupportY + lowerDeckThickness * 0.5f;
    float lowerDeckRailHeight = 0.16f;
    float lowerDeckRailWidth = 0.16f;
    float lowerDeckCrossWidth = chassisSize.x * 0.86f;
    float lowerDeckLength = chassisSize.z * 0.82f;
    float lowerDeckCrossZ = lowerDeckLength * 0.34f;
    float topDeckThickness = 0.10f;
    float topDeckY = lowerDeckY + 0.22f;
    float topDeckWidth = chassisSize.x * 1.05f;
    float topDeckLength = chassisSize.z * 0.90f;
    float mastBaseWidth = 0.78f;
    float mastBaseHeight = 0.24f;
    float mastBaseLength = 0.46f;
    float mastBaseY = topDeckY + topDeckThickness * 0.5f + mastBaseHeight * 0.5f;
    float mastBaseZ = -0.18f;
    float sensorStandRadius = 0.08f;
    float sensorStandHeight = 0.42f;
    float sensorStandSpacing = 0.42f;
    float sensorStandY = mastBaseY + mastBaseHeight * 0.5f + sensorStandHeight * 0.5f;
    float sensorEyeRadius = 0.15f;
    float sensorEyeLength = 0.30f;
    float sensorEyeY = sensorStandY + sensorStandHeight * 0.5f;
    float sensorEyeForwardOffset = 0.08f;

    glm::vec3 supportColor(0.33f, 0.28f, 0.22f);
    glm::vec3 mountColor(0.42f, 0.42f, 0.42f);
    glm::vec3 lowerDeckColor(0.52f, 0.49f, 0.43f);
    glm::vec3 topDeckColor(0.66f, 0.63f, 0.56f);
    glm::vec3 mastBaseColor(0.58f, 0.56f, 0.50f);
    glm::vec3 sensorStandColor(0.62f, 0.62f, 0.60f);
    glm::vec3 sensorEyeColor(0.68f, 0.68f, 0.66f);

    auto drawSupportCube = [&](const glm::mat4& localTransform, const glm::vec3& color) {
        if (structureVAO == nullptr) {
            return;
        }

        glBindVertexArray(structureVAO->getVAO());
        shader.setMat4("model", parentTransform * localTransform);
        shader.setVec3("objectColor", color);
        glDrawElements(GL_TRIANGLES, structureVAO->getVertexCount(), GL_UNSIGNED_INT, 0);
    };

    // Lower deck center plate that the support anchors visually connect into.
    glm::mat4 lowerDeckPlate = glm::mat4(1.0f);
    lowerDeckPlate = glm::translate(lowerDeckPlate, glm::vec3(0.0f, lowerDeckY, 0.0f));
    lowerDeckPlate = glm::scale(lowerDeckPlate, glm::vec3(chassisSize.x * 0.72f, lowerDeckThickness, lowerDeckLength));
    drawSupportCube(lowerDeckPlate, lowerDeckColor);

    // Side rails make the lower deck feel like a frame rather than a single box.
    for (float sideSign : { -1.0f, 1.0f }) {
        glm::mat4 sideRail = glm::mat4(1.0f);
        sideRail = glm::translate(
            sideRail,
            glm::vec3(sideSign * (sideAnchorX + 0.08f), lowerDeckY + 0.03f, 0.0f)
        );
        sideRail = glm::scale(sideRail, glm::vec3(lowerDeckRailWidth, lowerDeckRailHeight, lowerDeckLength * 0.96f));
        drawSupportCube(sideRail, lowerDeckColor);
    }

    // Front and rear cross members tie the side rails together.
    for (float zSign : { -1.0f, 1.0f }) {
        glm::mat4 crossBeam = glm::mat4(1.0f);
        crossBeam = glm::translate(crossBeam, glm::vec3(0.0f, lowerDeckY - 0.02f, zSign * lowerDeckCrossZ));
        crossBeam = glm::scale(crossBeam, glm::vec3(lowerDeckCrossWidth, lowerDeckThickness, 0.16f));
        drawSupportCube(crossBeam, lowerDeckColor);
    }

    // Top deck is the main rover platform carried by the lower frame.
    glm::mat4 topDeckPlate = glm::mat4(1.0f);
    topDeckPlate = glm::translate(topDeckPlate, glm::vec3(0.0f, topDeckY, 0.0f));
    topDeckPlate = glm::scale(topDeckPlate, glm::vec3(topDeckWidth, topDeckThickness, topDeckLength));
    drawSupportCube(topDeckPlate, topDeckColor);

    // Short deck risers make the layered chassis read as two separate structural levels.
    for (float sideSign : { -1.0f, 1.0f }) {
        glm::mat4 riser = glm::mat4(1.0f);
        riser = glm::translate(
            riser,
            glm::vec3(sideSign * (chassisSize.x * 0.22f), lowerDeckY + 0.11f, 0.0f)
        );
        riser = glm::scale(riser, glm::vec3(0.14f, 0.18f, chassisSize.z * 0.42f));
        drawSupportCube(riser, lowerDeckColor);
    }

    // Mast base is the first upper-deck subsystem mounted onto the top platform.
    glm::mat4 mastBase = glm::mat4(1.0f);
    mastBase = glm::translate(mastBase, glm::vec3(0.0f, mastBaseY, mastBaseZ));
    mastBase = glm::scale(mastBase, glm::vec3(mastBaseWidth, mastBaseHeight, mastBaseLength));
    drawSupportCube(mastBase, mastBaseColor);

    if (tire.cylinderVAO != nullptr) {
        glBindVertexArray(tire.cylinderVAO->getVAO());

        for (float sideSign : { -1.0f, 1.0f }) {
            glm::mat4 sensorStand = glm::mat4(1.0f);
            sensorStand = glm::translate(
                sensorStand,
                glm::vec3(sideSign * (sensorStandSpacing * 0.5f), sensorStandY, mastBaseZ)
            );
            sensorStand = glm::scale(
                sensorStand,
                glm::vec3(sensorStandRadius, sensorStandHeight, sensorStandRadius)
            );

            shader.setMat4("model", parentTransform * sensorStand);
            shader.setVec3("objectColor", sensorStandColor);
            glDrawElements(GL_TRIANGLES, tire.cylinderVAO->getVertexCount(), GL_UNSIGNED_INT, 0);
        }
    }

    if (coneVAO != nullptr) {
        glBindVertexArray(coneVAO->getVAO());

        for (float sideSign : { -1.0f, 1.0f }) {
            glm::mat4 sensorEye = glm::mat4(1.0f);
            sensorEye = glm::translate(
                sensorEye,
                glm::vec3(
                    sideSign * (sensorStandSpacing * 0.5f),
                    sensorEyeY,
                    mastBaseZ + sensorEyeForwardOffset
                )
            );
            sensorEye = glm::rotate(sensorEye, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            sensorEye = glm::scale(sensorEye, glm::vec3(sensorEyeRadius, sensorEyeLength, sensorEyeRadius));

            shader.setMat4("model", parentTransform * sensorEye);
            shader.setVec3("objectColor", sensorEyeColor);
            glDrawElements(GL_TRIANGLES, coneVAO->getVertexCount(), GL_UNSIGNED_INT, 0);
        }
    }

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
