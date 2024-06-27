#pragma once
#include <memory>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "engine.hpp"
#include "framebuffer.hpp"


namespace Tank
{
	class Node;
	class Scene;
}
class KeyInput;


struct WindowSettings
{
	ImGuiConfigFlags configFlags;
	ImGuiWindowFlags mainWinFlags;
	glm::ivec2 windowSize;
};


class Editor : public Tank::Application
{
private:
	GLFWwindow *m_window;

	/// <summary>
	/// Root node for system UI (Hierarchy, Inspector, etc.)
	/// </summary>
	std::unique_ptr<Tank::Node> m_system;
	std::unique_ptr<KeyInput> m_keyInput;
	std::unique_ptr<Tank::Scene> m_scene;
	
	std::unique_ptr<WindowSettings> m_settings;
public:
	/// <summary>
	/// Callback occurs when window size changes.
	/// </summary>
	static void onWindowSizeChange(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
	
	Editor();
	~Editor();
	void initGL();
	void initImGui();
	void initSystem();
	void loadScene();
	void run() override;
	void handleKeyInput();

	WindowSettings *getWindowSettings() const noexcept { return m_settings.get(); }
};
