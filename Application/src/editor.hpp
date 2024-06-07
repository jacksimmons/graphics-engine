#pragma once
#include <memory>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "engine.hpp"
#include "framebuffer.hpp"


namespace Tank
{
	class Node;
	class SceneView;
}
class KeyInput;


struct EditorSettings
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
	/// Root node for editor UI (Hierarchy, Inspector, etc.)
	/// </summary>
	std::shared_ptr<Tank::Node> m_uiRoot;
	std::unique_ptr<KeyInput> m_keyInput;
	
	EditorSettings m_settings;

public:
	/// <summary>
	/// Callback occurs when window size changes.
	/// </summary>
	static void onWindowSizeChange(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
	
	Editor();
	~Editor();
	void initGL();
	void initImGui();
	void generateSceneThenInitInput();
	void run() override;
	void handleKeyInput();
};
