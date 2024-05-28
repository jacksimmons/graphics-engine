#pragma once
#include <GLFW/glfw3.h>
#include "engine.hpp"


class Editor : public Tank::Application
{
private:
	GLFWwindow *m_window;
public:
	/// <summary>
	/// Callback occurs when window size changes.
	/// </summary>
	static void onFramebufferSizeChange(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
	
	Editor();
	~Editor();
	void initGL();
	void initImGui();
	void run() override;
	void cleanup();
};
