#pragma once
#include <map>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.hpp"


enum class KeyState
{
	Pressed,
	Held,
	Released,
	NotPressed
};


class KeyInput
{
private:
	static std::vector<KeyInput *> s_instances;
	static int s_renderMode;
	std::map<int, KeyState> m_keys;

	static void callback(GLFWwindow *win, int key, int scancode, int action, int mods);
	void setKeyState(int key, KeyState state);
public:
	KeyInput(std::vector<int> monitoredKeys);
	~KeyInput();

	static void setupKeyInputs(GLFWwindow *window);
	KeyState getKeyState(int key);
	void cycleRenderMode();
	void update();
};