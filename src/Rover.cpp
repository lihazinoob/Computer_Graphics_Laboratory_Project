#include "../include/Rover.h"

Rover::Rover(VertexObject* cylVAO, VertexObject* torVAO, float tireRadius, float tireWidth, float numberOfSpokes)
	:tire(cylVAO, torVAO, tireRadius, tireWidth, numberOfSpokes)
{
	chassisSize = glm::vec3(2.0f, 0.5f, 5.0f);
}

void Rover::Draw(Shader& shader, glm::mat4 parentTransform) {
	
	// Draw the 4 tires
	// Using the basic geometry to put the tires perfectly on the corners
    float xOffset = (chassisSize.x / 2.0f) + (tire.tireWidth / 2.0f) + 0.05f; // Push out to sides
    float zOffset = (chassisSize.z / 2.0f) - tire.tireRadius - 0.1f;         // Push to front/back
    float yOffset = tire.tireRadius; // Center of tire sits exactly at radius height (so bottom touches floor)

    glm::vec3 tirePositions[4] = {
        glm::vec3(xOffset, yOffset,  zOffset), // Front-Right
        glm::vec3(-xOffset, yOffset,  zOffset), // Front-Left
        glm::vec3(xOffset, yOffset, -zOffset), // Back-Right
        glm::vec3(-xOffset, yOffset, -zOffset)  // Back-Left
    };

    for (int i = 0; i < 4; i++) {
        glm::mat4 tireLocal = glm::mat4(1.0f);

        // Translate the tires to calculated cornered positon
        tireLocal = glm::translate(tireLocal, tirePositions[i]);

        // B. Stand the tire up (Native cylinder is flat on Y. We rotate 90 on Z to align the axle to X)
        tireLocal = glm::rotate(tireLocal, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // A. Spin the tire around its local axle (Original Y axis)
        tireLocal = glm::rotate(tireLocal,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Hierarchical Magic: Multiply child local by the master parent!
        glm::mat4 tireWorld = parentTransform * tireLocal;

        // Tell the tire to draw itself using this calculated world matrix
        tire.Draw(shader, tireWorld);

    }

}