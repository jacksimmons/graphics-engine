#include "framebuffer.hpp"


/// <summary>
/// Callback occurs when window size changes.
/// </summary>
void onFramebufferSizeChange(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}