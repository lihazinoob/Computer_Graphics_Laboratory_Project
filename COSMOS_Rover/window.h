#pragma once

#ifndef WINDOW_H
#define WINDOW_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>

// framebuffer resize callback declaration
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* createWindowAndInitGL(
    int width,
    int height,
    const char* title
);


#endif
