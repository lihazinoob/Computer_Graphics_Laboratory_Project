#include "../include/window.h"
#include "../include/generateObjects.h"
#include "../include/vertexObject.h"
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "../include/Shader.h"
#include "../include/utility_functions.h"
#include "../include/Camera.h"
#include "../include/DirectionalLight.h"
#include "../include/SpotLight.h"
#include "../include/Rover.h"




// ==================== Window Size ====================
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));


int main() {
    GLFWwindow* window = createWindowAndInitGL(SCR_WIDTH, SCR_HEIGHT, "COSMOS-Rover | Assignment B1");
    if (!window) return -1;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // Load the shader files
    const char* vertexShaderFilePath = "shaders/vertexShader.vs";
    const char* fragmentShaderFilePath = "shaders/fragmentShader.fs";
    Shader myShader(vertexShaderFilePath, fragmentShaderFilePath);


    // Create a Sun(Directional Light)
    glm::vec3 sunDirection(-0.8f, -1.0f, 1.0f);
    glm::vec3 sunAmbient(0.1f, 0.1f, 0.1f); // Dark shadows
    glm::vec3 sunDiffuse(0.8f, 0.8f, 0.8f); // Bright sunlight
    glm::vec3 sunSpecular(0.2f, 0.2f, 0.2f); // Low specular for rough dirt
    DirectionalLight sun(sunDirection, sunAmbient, sunDiffuse, sunSpecular);
    SpotLight roverLamp(
        glm::vec3(0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.02f, 0.02f, 0.015f),
        glm::vec3(0.95f, 0.92f, 0.82f),
        glm::vec3(1.0f, 0.98f, 0.90f),
        glm::cos(glm::radians(13.0f)),
        glm::cos(glm::radians(20.0f)),
        1.0f,
        0.09f,
        0.032f
    );



    // Create a Terrain using the heatMap
    const char* heightMapPath = "utils/iceland_heightmap.png";
    std::vector<float> terrainHeights;
    int tWidth, tHeight;
    std::vector<float>terrainVertices;
    std::vector<unsigned int>terrainIndices;
    generateTerrainFromHeightMap(heightMapPath, 32.0f, 0.0f, terrainVertices, terrainIndices, terrainHeights,tWidth,tHeight);
    // Now bind the corresponding VAO,VBO and EBO
    VertexObject terrainVAO = createVAOWithPositionNormalAndTexCoord(terrainVertices, terrainIndices);
    unsigned int terrainTexture = loadTexture2D("utils/rock_ground_diff_4k.jpg");

    

    // Create a Tire using the cylinder and torus generation
    std::vector<float>tireVertices;
    std::vector<unsigned int> tireIndices;
    generateCylinder(1.0f, 1.0f, 36, tireVertices, tireIndices);
    VertexObject baseCylinderVAO = createVAOWithPositionAndNormal(tireVertices, tireIndices);
    std::vector<float> torusVerts;
    std::vector<unsigned int> torusInds;
    generateTorus(0.72f, 0.28f, 36, 18, torusVerts, torusInds);
    VertexObject baseTorusVAO = createVAOWithPositionAndNormal(torusVerts, torusInds);

    std::vector<float> coneVertices;
    std::vector<unsigned int> coneIndices;
    generateCone(1.0f, 1.0f, 36, coneVertices, coneIndices);
    VertexObject baseConeVAO = createVAOWithPositionNormalAndTexCoord(coneVertices, coneIndices);
    unsigned int eyeConeTexture = loadTexture2D("utils/birds.jpg");
    
    // Create a shared cube mesh for rover support structures
    std::vector<float> cubeVertices;
    std::vector<unsigned int> cubeIndices;
    generateUnitCubeUsingPositionAndNormal(cubeVertices, cubeIndices);
    VertexObject structureVAO = createVAOWithPositionAndNormal(cubeVertices, cubeIndices);

    // Now create the parent rover using the tires
    float tireRadius = 0.3f;
    float tireWidth = 0.1f;

    Rover rover(&structureVAO, &baseConeVAO, &baseCylinderVAO, &baseTorusVAO, eyeConeTexture, tireRadius, tireWidth, 12);
    

    float terrainScale = 0.5f; // Scale that is applied so that the terrain does not stretch too much
    float playerEyeHeight = 2.0f; // How tall the camera is standing above the dirt
    bool togglePressedLastFrame = false;
    bool directionalTogglePressedLastFrame = false;
    bool spotTogglePressedLastFrame = false;
    bool directionalLightEnabled = true;
    bool spotLightEnabled = true;

    // Timing variables for smooth, frame-independent movement
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the Screen and Depth Buffer
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // Calculate deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Toggle between walking mode and free top-view mode.
        bool togglePressedNow = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
        if (togglePressedNow && !togglePressedLastFrame) {
            if (camera.viewMode == GROUND_VIEW) {
                camera.SetViewMode(TOP_DOWN_VIEW);
                camera.cameraPositon.y += 14.0f;
            }
            else {
                camera.SetViewMode(GROUND_VIEW);
            }
        }
        togglePressedLastFrame = togglePressedNow;

        bool directionalTogglePressedNow = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
        if (directionalTogglePressedNow && !directionalTogglePressedLastFrame) {
            directionalLightEnabled = !directionalLightEnabled;
        }
        directionalTogglePressedLastFrame = directionalTogglePressedNow;

        bool spotTogglePressedNow = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
        if (spotTogglePressedNow && !spotTogglePressedLastFrame) {
            spotLightEnabled = !spotLightEnabled;
        }
        spotTogglePressedLastFrame = spotTogglePressedNow;

        // 1. PROCESS INPUT using the Camera class
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
        if (camera.viewMode == TOP_DOWN_VIEW) {
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
        }
        else {
            float terrainY = getTerrainHeight(camera.cameraPositon.x, camera.cameraPositon.z, terrainHeights, tWidth, tHeight, terrainScale);
            camera.cameraPositon.y = terrainY * terrainScale + playerEyeHeight;
        }

        
        
        
        // View Matrix
        glm::mat4 view = camera.GetViewMatrix();

        int currentWidth, currentHeight;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);

        if (currentHeight == 0) {
            currentHeight = 1;
        }
        // Projection Matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)currentWidth / (float)currentHeight, 0.1f, 100.0f);

        // Use the shader
        myShader.use();
        // Set those matrices to the shader program
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);
        myShader.setInt("diffuseTexture", 0);

        
        myShader.setVec3("viewPos", camera.cameraPositon);
        if (directionalLightEnabled) {
            sun.ApplyToShader(myShader);
        }
        else {
            DirectionalLight disabledSun(
                sunDirection,
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                glm::vec3(0.0f)
            );
            disabledSun.ApplyToShader(myShader);
        }
        

        
        // Now I can draw the terrain
        glm::mat4 terrainModel = glm::mat4(1.0f);
        terrainModel = glm::scale(terrainModel, glm::vec3(terrainScale, terrainScale, terrainScale));
        myShader.setMat4("model", terrainModel);
        glm::vec3 terrainColor = glm::vec3(0.8f, 0.5f, 0.2f);
        myShader.setVec3("objectColor", terrainColor);
        myShader.setBool("useTexture", true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, terrainTexture);
        glBindVertexArray(terrainVAO.getVAO());
        glDrawElements(GL_TRIANGLES, terrainVAO.getVertexCount(), GL_UNSIGNED_INT, 0);
        
        // ==========================================
        // RENDER: HIERARCHICAL ROVER
        // ==========================================
        // 1. Where do we want the whole rover assembly to be?
        float targetWorldX = rover.position.x;
        float targetWorldZ =rover.position.z;

        rover.Update(window, deltaTime);
        float rawTerrainY = getTerrainHeight(targetWorldX, targetWorldZ, terrainHeights, tWidth, tHeight, terrainScale);
        float worldTerrainY = rawTerrainY * terrainScale;

        glm::mat4 roverLocation = glm::mat4(1.0f);
        // Place the rover exactly on the terrain!
        roverLocation = glm::translate(roverLocation, glm::vec3(targetWorldX, worldTerrainY, targetWorldZ));
        // Apply the base facing offset plus user-controlled rover yaw.
        roverLocation = glm::rotate(roverLocation, glm::radians(-135.0f + rover.yawDegrees), glm::vec3(0.0f, 1.0f, 0.0f));

        // Spotlight source is emitted from the base of the rear lamp cone on the rover.
        float lowerDeckThickness = 0.12f;
        float topSupportY = tireRadius + 0.52f;
        float lowerDeckY = topSupportY + lowerDeckThickness * 0.5f;
        float topDeckThickness = 0.10f;
        float topDeckY = lowerDeckY + 0.22f;
        float topDeckLength = 3.0f * 0.90f;
        float lampStandHeight = 0.95f;
        float lampStandZ = -topDeckLength * 0.28f;
        float lampStandY = topDeckY + topDeckThickness * 0.5f + lampStandHeight * 0.5f;
        float lampConeLength = 0.24f;
        float lampConeY = lampStandY + lampStandHeight * 0.5f;
        float lampConeForwardOffset = 0.12f;

        float lampTiltRadians = glm::radians(28.0f);
        glm::vec4 lampBaseLocal = glm::vec4(0.0f, lampConeY, lampStandZ + lampConeForwardOffset + lampConeLength * 0.5f, 1.0f);
        glm::vec4 lampDirectionLocal = glm::vec4(
            0.0f,
            -glm::sin(lampTiltRadians),
            glm::cos(lampTiltRadians),
            0.0f
        );
        roverLamp.Position = glm::vec3(roverLocation * lampBaseLocal);
        roverLamp.Direction = glm::normalize(glm::vec3(roverLocation * lampDirectionLocal));
        if (spotLightEnabled) {
            roverLamp.ApplyToShader(myShader);
        }
        else {
            SpotLight disabledSpot(
                roverLamp.Position,
                roverLamp.Direction,
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                roverLamp.CutOff,
                roverLamp.OuterCutOff,
                roverLamp.Constant,
                roverLamp.Linear,
                roverLamp.Quadratic
            );
            disabledSpot.ApplyToShader(myShader);
        }

        // Draw the support structures and all 4 spinning tires
        myShader.setBool("useTexture", false);
        rover.Draw(myShader, roverLocation);

        
        
       
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    
    // Clean Up  
    glDeleteVertexArrays(1, &terrainVAO.getVAO());
    glDeleteBuffers(1, &terrainVAO.getVBO());
    glDeleteBuffers(1, &terrainVAO.getEBO());

    glDeleteVertexArrays(1, &baseCylinderVAO.getVAO());
    glDeleteBuffers(1, &baseCylinderVAO.getVBO());
    glDeleteBuffers(1, &baseCylinderVAO.getEBO());

    glDeleteVertexArrays(1, &baseTorusVAO.getVAO());
    glDeleteBuffers(1, &baseTorusVAO.getVBO());
    glDeleteBuffers(1, &baseTorusVAO.getEBO());

    glDeleteVertexArrays(1, &baseConeVAO.getVAO());
    glDeleteBuffers(1, &baseConeVAO.getVBO());
    glDeleteBuffers(1, &baseConeVAO.getEBO());

    glDeleteVertexArrays(1, &structureVAO.getVAO());
    glDeleteBuffers(1, &structureVAO.getVBO());
    glDeleteBuffers(1, &structureVAO.getEBO());
    glDeleteTextures(1, &terrainTexture);
    glDeleteTextures(1, &eyeConeTexture);

    glfwTerminate();

	return 0;
}
