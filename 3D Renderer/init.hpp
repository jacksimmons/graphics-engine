#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "key_input.hpp"


GLFWwindow *initGLFW(int w, int h, char *name);
GLFWwindow *initWindow(int w, int h, char *name);
bool initGLAD();