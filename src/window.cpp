#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0,width, height);
}




GLFWwindow* createWindowAndInitGL(int width, int height, const char* title) {
	if (!glfwInit()) {
		std::cout << "Failed to initalize the GLFW window\n";
		return nullptr;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return nullptr;
	}

	return window;

}