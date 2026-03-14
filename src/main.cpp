
#include "../include/window.h"
#include "../include/generateObjects.h"
#include "../include//vertexObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "../include/Shader.h"

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
    std::vector<float>terrainVertices;
    std::vector<unsigned int>terrainIndices;
    generateTerrainFromHeightMap(heightMapPath, 32.0f, 0.0f, terrainVertices, terrainIndices);


    // Now bind the corresponding VAO,VBO and EBO
    VertexObject terrainVAO = createVAOWithPositionAndNormal(terrainVertices, terrainIndices);


    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the Screen and Depth Buffer
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader
        myShader.use();

        // Define the model matrices(only Rotation)
        // Parent Matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 rotationAxis(0.5f, 1.0f, 0.0f);

        // Scale the massive terrain into a shrinked one
        glm::vec3 scalingFactor(0.05f, 0.05f, 0.05f);
        model = glm::scale(model, scalingFactor);


        // The camera will stay at three unit back of z-axis
        glm::mat4 view = glm::mat4(1.0f);
        glm::vec3 cameraPosition(0.0f, 0.0f, -20.0f);
        view = glm::translate(view, cameraPosition);
        // Look slightly down at the terrain
        view = glm::rotate(view, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));



        // For camera I am using Perpective Projection
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