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

void Tire::DrawCylinderPart(
    Shader& shader,
    const glm::mat4& parentTransform,
    const glm::vec3& translation,
    const glm::vec3& scale,
    const glm::vec3& color
) {
    glm::mat4 localTransform = glm::mat4(1.0f);
    localTransform = glm::translate(localTransform, translation);
    localTransform = glm::scale(localTransform, scale);

    shader.setMat4("model", parentTransform * localTransform);
    shader.setVec3("objectColor", color);
    glDrawElements(GL_TRIANGLES, cylinderVAO->getVertexCount(), GL_UNSIGNED_INT, 0);
}

void Tire::Draw(Shader& shader, glm::mat4 parentTransform) {
    // Build a chunky rover-style wheel.
    // The outer rubber uses the shared torus mesh, while the inner mechanical parts use cylinders.
    // The wheel's axle remains on the local Y axis so the existing Rover transforms still work.
    float tireOuterRadius = tireRadius;
    float tireInnerRadius = tireRadius * 0.68f;
    float tireBodyWidth = tireWidth * 1.35f;
    float rimFaceWidth = tireWidth * 0.42f;
    float hubRadius = tireRadius * 0.18f;
    float hubWidth = tireWidth * 0.78f;

    // Main rubber body from torus geometry
    glBindVertexArray(torusVAO->getVAO());
    glm::mat4 rubberLocal = glm::mat4(1.0f);
    rubberLocal = glm::scale(rubberLocal, glm::vec3(tireOuterRadius, tireBodyWidth, tireOuterRadius));
    shader.setMat4("model", parentTransform * rubberLocal);
    shader.setVec3("objectColor", rubberColor);
    glDrawElements(GL_TRIANGLES, torusVAO->getVertexCount(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(cylinderVAO->getVAO());

    // Recessed metal rim on the front face
    DrawCylinderPart(
        shader,
        parentTransform,
        glm::vec3(0.0f, tireBodyWidth * 0.18f, 0.0f),
        glm::vec3(tireInnerRadius, rimFaceWidth, tireInnerRadius),
        rimColor
    );

    // Recessed metal rim on the back face
    DrawCylinderPart(
        shader,
        parentTransform,
        glm::vec3(0.0f, -tireBodyWidth * 0.18f, 0.0f),
        glm::vec3(tireInnerRadius, rimFaceWidth, tireInnerRadius),
        rimColor
    );

    // Central hub protruding slightly through the rim
    DrawCylinderPart(
        shader,
        parentTransform,
        glm::vec3(0.0f),
        glm::vec3(hubRadius, hubWidth, hubRadius),
        glm::vec3(0.48f, 0.48f, 0.48f)
    );
}
