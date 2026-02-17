#include "window.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertexObject.h"
#include "generateObjects.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float ambientStrength = 1.0;

void processInput(GLFWwindow* window);


int main() {
    GLFWwindow* window = createWindowAndInitGL(SCR_WIDTH, SCR_HEIGHT, "Project");
    if (!window) {
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");


    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.2f,
         0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.2f,
         0.5f,  0.5f, -0.5f,  0.8f, 0.2f, 0.2f,
        -0.5f,  0.5f, -0.5f,  0.8f, 0.2f, 0.2f,
        // Front face
        -0.5f, -0.5f,  0.5f,  0.2f, 0.8f, 0.2f,
         0.5f, -0.5f,  0.5f,  0.2f, 0.8f, 0.2f,
         0.5f,  0.5f,  0.5f,  0.2f, 0.8f, 0.2f,
        -0.5f,  0.5f,  0.5f,  0.2f, 0.8f, 0.2f,
        // Left face
        -0.5f,  0.5f,  0.5f,  0.2f, 0.2f, 0.8f,
        -0.5f,  0.5f, -0.5f,  0.2f, 0.2f, 0.8f,
        -0.5f, -0.5f, -0.5f,  0.2f, 0.2f, 0.8f,
        -0.5f, -0.5f,  0.5f,  0.2f, 0.2f, 0.8f,
        // Right face
         0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.2f,
         0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.2f,
         0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.2f,
         0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.2f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f,
         0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f,
         0.5f, -0.5f,  0.5f,  0.8f, 0.2f, 0.8f,
        -0.5f, -0.5f,  0.5f,  0.8f, 0.2f, 0.8f,
        // Top face
        -0.5f,  0.5f, -0.5f,  0.2f, 0.8f, 0.8f,
         0.5f,  0.5f, -0.5f,  0.2f, 0.8f, 0.8f,
         0.5f,  0.5f,  0.5f,  0.2f, 0.8f, 0.8f,
        -0.5f,  0.5f,  0.5f,  0.2f, 0.8f, 0.8f
    };

    unsigned int cubeIndices[] = {
        0, 1, 2, 2, 3, 0,       // Back face
        4, 5, 6, 6, 7, 4,       // Front face
        8, 9, 10, 10, 11, 8,    // Left face
        12, 13, 14, 14, 15, 12, // Right face
        16, 17, 18, 18, 19, 16, // Bottom face
        20, 21, 22, 22, 23, 20  // Top face
    };

    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;

    generateSphere(0.5f, 36, 18, sphereVertices, sphereIndices);

    VertexObject sphereVAO = createVAO(sphereVertices.data(),
        sphereVertices.size() * sizeof(float),
        sphereIndices.data(),
        sphereIndices.size() * sizeof(unsigned int));

    VertexObject cubeVAO = createVAO(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices));


    while (!glfwWindowShouldClose(window)) {


        processInput(window);
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        ourShader.use();
        ourShader.setVec3("lightColor", lightColor);
        ourShader.setFloat("ambientStrength", ambientStrength);


        // Common View and Projection matrices
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("viewMatrix", view);
        ourShader.setMat4("projectionMatrix", projection);

        // --- DRAW THE CUBE ---
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(-1.2f, 0.0f, 0.0f)); // Move left
        cubeModel = glm::rotate(cubeModel, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        ourShader.setMat4("modelTransformationMatrix", cubeModel);

        glBindVertexArray(cubeVAO.VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // --- DRAW THE SPHERE ---
        glm::mat4 sphereModel = glm::mat4(1.0f);
        sphereModel = glm::translate(sphereModel, glm::vec3(1.2f, 0.0f, 0.0f)); // Move right
        sphereModel = glm::rotate(sphereModel, (float)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.setMat4("modelTransformationMatrix", sphereModel);

        glBindVertexArray(sphereVAO.VAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)sphereIndices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO.VAO);
    glDeleteBuffers(1, &cubeVAO.VBO);
    glDeleteBuffers(1, &cubeVAO.EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        ambientStrength = 0.1f;
    }
    else {
        ambientStrength = 1.0f;
    }
}