
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

// ==================== Window Size ====================
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));


int main() {
    GLFWwindow* window = createWindowAndInitGL(SCR_WIDTH, SCR_HEIGHT, "COSMOS-Rover | Assignment B1");
    if (!window) return -1;

    //glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);


    // Load the shader files
    const char* vertexShaderFilePath = "shaders/vertexShader.vs";
    const char* fragmentShaderFilePath = "shaders/fragmentShader.fs";
    Shader myShader(vertexShaderFilePath, fragmentShaderFilePath);

    // Create a Terrain using the heatMap
    const char* heightMapPath = "utils/iceland_heightmap.png";
    std::vector<float> terrainHeights;
    int tWidth, tHeight;
    std::vector<float>terrainVertices;
    std::vector<unsigned int>terrainIndices;
    generateTerrainFromHeightMap(heightMapPath, 32.0f, 0.0f, terrainVertices, terrainIndices, terrainHeights,tWidth,tHeight);


    // Now bind the corresponding VAO,VBO and EBO
    VertexObject terrainVAO = createVAOWithPositionAndNormal(terrainVertices, terrainIndices);

    float globalScale = 0.05f; // The scale you applied to your model
    float playerEyeHeight = 0.2f; // How tall the camera is standing above the dirt

    // Timing variables for smooth, frame-independent movement
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        
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
        float terrainY = getTerrainHeight(camera.cameraPositon.x,camera.cameraPositon.z, terrainHeights, tWidth, tHeight, globalScale);
        camera.cameraPositon.y = terrainY + playerEyeHeight;



        // Clear the Screen and Depth Buffer
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader
        myShader.use();

        // Model Matrix (Static now, we are moving the camera, not the world)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(globalScale, globalScale, globalScale));

        // View Matrix
        glm::mat4 view = camera.GetViewMatrix();


        // Projection Matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        // Set those matrices to the shader program
        myShader.setMat4("model", model);
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);

        // Now I can draw the cube
        glBindVertexArray(terrainVAO.getVAO());
        glDrawElements(GL_TRIANGLES, terrainVAO.getVertexCount(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    
    // Clean Up  
    glDeleteVertexArrays(1, &terrainVAO.getVAO());
    glDeleteBuffers(1, &terrainVAO.getVBO());
    glDeleteBuffers(1, &terrainVAO.getEBO());

    glfwTerminate();

	return 0;
}