
#include "../include/window.h"
#include "../include/generateObjects.h"
#include "../include/vertexObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "../include/Shader.h"
#include "../include/utility_functions.h"

// ==================== Window Size ====================
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;


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

    // Setup Camera variables
    glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f);
    float cameraSpeed = 0.1f;
    float globalScale = 0.05f; // The scale you applied to your model
    float playerEyeHeight = 0.2f; // How tall the camera is standing above the dirt


    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // 1. PROCESS INPUT (WASD to move X and Z)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition.z -= cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition.z += cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition.x -= cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition.x += cameraSpeed;

        // 2. TERRAIN FOLLOWING (Calculate Y based on X and Z)
        float terrainY = getTerrainHeight(cameraPosition.x, cameraPosition.z, terrainHeights, tWidth, tHeight, globalScale);
        cameraPosition.y = terrainY + playerEyeHeight;


        // Clear the Screen and Depth Buffer
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader
        myShader.use();

        // Model Matrix (Static now, we are moving the camera, not the world)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(globalScale, globalScale, globalScale));

        // View Matrix
        glm::mat4 view = glm::mat4(1.0f);
        // Look down slightly to see the ground
        view = glm::rotate(view, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // Move opposite to camera position (OpenGL convention)
        view = glm::translate(view, -cameraPosition);

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