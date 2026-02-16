#include "window.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


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

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glm::vec3 lightColor(0.0f, 1.0f, 1.0f);
        ourShader.use();
        ourShader.setVec3("lightColor", lightColor);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.setMat4("modelTransformationMatrix", model);
        ourShader.setMat4("viewMatrix", view);
        ourShader.setMat4("projectionMatrix", projection);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}