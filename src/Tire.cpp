#include "../include/Tire.h"

Tire::Tire(VertexObject* sharedCylinder,VertexObject* sharedTorus, float radius, float width, int spokes) {
    cylinderVAO = sharedCylinder;
    torusVAO = sharedTorus;
    tireRadius = radius;
    tireWidth = width;
    numSpokes = spokes;

    rubberColor = glm::vec3(0.05f, 0.05f, 0.05f); // Very dark grey/black
    rimColor = glm::vec3(0.7f, 0.7f, 0.7f);       // Metallic silver
}

void Tire::Draw(Shader& shader, glm::mat4 parentTransform) {
    glBindVertexArray(cylinderVAO->getVAO());

    // 1. The Centered HUB
    glm::mat4 hubLocal = glm::mat4(1.0f);
    // Scale it to be a small, thick disk in the center
    float hubRadius = tireRadius * 0.3f;
    float hubWidth = tireWidth * 1.05f; // Slightly wider than the tire
    hubLocal = glm::scale(hubLocal, glm::vec3(hubRadius, hubWidth, hubRadius));

    // Multiply by parent matrix! 
    glm::mat4 hubWorld = parentTransform * hubLocal;
    shader.setMat4("model", hubWorld);
    shader.setVec3("objectColor", rimColor);
    glDrawElements(GL_TRIANGLES, cylinderVAO->getVertexCount(), GL_UNSIGNED_INT, 0);

    // ==========================================
    // 2. Draw the SPOKES
    // ==========================================
    float angleStep = 360.0f / numSpokes;
    float spokeRadius = 0.02f; // Very thin
    float spokeLength = tireRadius * 0.8f; // Long enough to reach the outer rim

    for (int i = 0; i < numSpokes; i++) {
        glm::mat4 spokeLocal = glm::mat4(1.0f);

        // A. Rotate to the correct angle pointing outward from center
        spokeLocal = glm::rotate(spokeLocal, glm::radians(i * angleStep), glm::vec3(0.0f, 1.0f, 0.0f));

        // B. Move it outward so it starts at the hub and ends at the tire
        spokeLocal = glm::translate(spokeLocal, glm::vec3(spokeLength / 2.0f, 0.0f, 0.0f));

        // C. Rotate the cylinder 90 degrees so it lays flat along X axis
        spokeLocal = glm::rotate(spokeLocal, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // D. Scale it to look like a spoke
        spokeLocal = glm::scale(spokeLocal, glm::vec3(spokeRadius, spokeLength, spokeRadius));

        glm::mat4 spokeWorld = parentTransform * spokeLocal;

        shader.setMat4("model", spokeWorld);
        shader.setVec3("objectColor", rimColor);
        glDrawElements(GL_TRIANGLES, cylinderVAO->getVertexCount(), GL_UNSIGNED_INT, 0);
    }

    // ==========================================
    // 3. Draw the TREAD using TORUS
    // ==========================================
    glBindVertexArray(torusVAO->getVAO());
    glm::mat4 rubberLocal = glm::mat4(1.0f);

    // The torus is naturally generated lying flat on the XZ plane.
    // The cylinder lays flat too, so they align perfectly.
    // Scale the raw unit torus to match our desired tire size
    rubberLocal = glm::scale(rubberLocal, glm::vec3(tireRadius, tireWidth, tireRadius));

    glm::mat4 rubberWorld = parentTransform * rubberLocal;
    shader.setMat4("model", rubberWorld);
    shader.setVec3("objectColor", rubberColor);
    glDrawElements(GL_TRIANGLES, torusVAO->getVertexCount(), GL_UNSIGNED_INT, 0);


}