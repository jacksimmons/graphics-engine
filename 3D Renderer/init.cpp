#include <stdio.h>
#include <stddef.h>
#include "init.hpp"
#include "file.hpp"
#include "framebuffer.hpp"


/// <summary>
/// Initialises GLFW (GL framework), and sets window hints.
/// </summary>
GLFWwindow *initGLFW(int w, int h, char *name)
{
	if (!glfwInit())
	{
		printf("Failed to init GLFW.");
		glfwTerminate();
		return NULL;
	}

	// Set GL version hint
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = initWindow(w, h, name);

	// Initialise callbacks
	KeyInput::setupKeyInputs(window);
	glfwSetFramebufferSizeCallback(window, onFramebufferSizeChange);

	return window;
}



/// <summary>
/// Initialises the GLFW window.
/// </summary>
/// <param name="width">Window width.</param>
/// <param name="height">Window height.</param>
/// <returns>The window initialised.</returns>
GLFWwindow *initWindow(int width, int height, char *name)
{
	GLFWwindow *window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window.");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	return window;
}


/// <summary>
/// Initialises GLAD (GL loader).
/// </summary>
bool initGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to init GLAD.");
		return false;
	}

	return true;
}