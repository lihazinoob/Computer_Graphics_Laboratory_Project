#include "window.h"
#include "shader.h"



const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;






int main() {
    GLFWwindow* window = createWindowAndInitGL(SCR_WIDTH, SCR_HEIGHT, "Assignment 02 Final");
    if (!window) {
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");


    // Realistic plane colors
    float cubeVertices[] = {
        // x      y      z        r     g     b
        -0.5f, -0.5f, -0.5f,     0.95f, 0.95f, 0.97f,
         0.5f, -0.5f, -0.5f,     0.90f, 0.90f, 0.92f,
         0.5f,  0.5f, -0.5f,     0.95f, 0.95f, 0.97f,
        -0.5f,  0.5f, -0.5f,     0.95f, 0.95f, 0.97f,
        -0.5f, -0.5f,  0.5f,     0.95f, 0.95f, 0.97f,
         0.5f, -0.5f,  0.5f,     0.90f, 0.90f, 0.92f,
         0.5f,  0.5f,  0.5f,     0.95f, 0.95f, 0.97f,
        -0.5f,  0.5f,  0.5f,     0.95f, 0.95f, 0.97f
    };

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

    }



}