#include "../include/window.h"
#include "../include/generateObjects.h"
#include "../include/vertexObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "../include/Shader.h"
#include "../include/utility_functions.h"
#include "../include/Camera.h"
#include "../include/DirectionalLight.h"
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



    // Create a Terrain using the heatMap
    const char* heightMapPath = "utils/iceland_heightmap.png";
    std::vector<float> terrainHeights;
    int tWidth, tHeight;
    std::vector<float>terrainVertices;
    std::vector<unsigned int>terrainIndices;
    generateTerrainFromHeightMap(heightMapPath, 32.0f, 0.0f, terrainVertices, terrainIndices, terrainHeights,tWidth,tHeight);
    // Now bind the corresponding VAO,VBO and EBO
    VertexObject terrainVAO = createVAOWithPositionAndNormal(terrainVertices, terrainIndices);

    

    // Create a Tire using the cylinder and torus generation
    std::vector<float>tireVertices;
    std::vector<unsigned int> tireIndices;
    generateCylinder(1.0f, 1.0f, 36, tireVertices, tireIndices);
    VertexObject baseCylinderVAO = createVAOWithPositionAndNormal(tireVertices, tireIndices);
    std::vector<float> torusVerts;
    std::vector<unsigned int> torusInds;
    generateTorus(0.8f, 0.2f, 36, 18, torusVerts, torusInds);
    VertexObject baseTorusVAO = createVAOWithPositionAndNormal(torusVerts, torusInds);
    
    
    // Now create the parent rover using the tires
    float tireRadius = 0.3f;
    float tireWidth = 0.1f;
    Rover rover(&baseCylinderVAO, &baseTorusVAO, tireRadius, tireWidth, 12);
    



    float terrainScale = 0.5f; // Scale that is applied so that the terrain does not stretch too much
    float playerEyeHeight = 0.8f; // How tall the camera is standing above the dirt

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

        // 1. PROCESS INPUT using the Camera class
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);

        // 2. TERRAIN FOLLOWING
        float terrainY = getTerrainHeight(camera.cameraPositon.x,camera.cameraPositon.z, terrainHeights, tWidth, tHeight, terrainScale);
        camera.cameraPositon.y = terrainY * terrainScale + playerEyeHeight;

        
        
        
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

        
        myShader.setVec3("viewPos", camera.cameraPositon);
        sun.ApplyToShader(myShader);


        
        // Now I can draw the terrain
        glm::mat4 terrainModel = glm::mat4(1.0f);
        terrainModel = glm::scale(terrainModel, glm::vec3(terrainScale, terrainScale, terrainScale));
        myShader.setMat4("model", terrainModel);
        glm::vec3 terrainColor = glm::vec3(0.8f, 0.5f, 0.2f);
        myShader.setVec3("objectColor", terrainColor);
        glBindVertexArray(terrainVAO.getVAO());
        glDrawElements(GL_TRIANGLES, terrainVAO.getVertexCount(), GL_UNSIGNED_INT, 0);
        
        // ==========================================
        // RENDER: HIERARCHICAL ROVER
        // ==========================================
        // 1. Where do we want the whole rover assembly to be?
        float targetWorldX = 0.0f;
        float targetWorldZ = -5.0f;
        float rawTerrainY = getTerrainHeight(targetWorldX, targetWorldZ, terrainHeights, tWidth, tHeight, terrainScale);
        float worldTerrainY = rawTerrainY * terrainScale;

        glm::mat4 roverLocation = glm::mat4(1.0f);
        // Place the rover exactly on the terrain!
        roverLocation = glm::translate(roverLocation, glm::vec3(targetWorldX, worldTerrainY, targetWorldZ));
        // Rotate body to face forward if necessary...

        // Draw the chassis AND all 4 spinning tires
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
    glfwTerminate();

	return 0;
}