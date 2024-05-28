#pragma once
#include <GLFW/glfw3.h>
#include "engine.hpp"


class Editor : public Tank::Application
{
private:
	GLFWwindow *m_window;
public:
	Editor();
	~Editor();
	void run() override;
};
