#pragma once
#include <memory>
#include <GLFW/glfw3.h>
#include "engine.hpp"
#include "framebuffer.hpp"


namespace Tank
{
	class Node;
}
class SceneView;

class Editor : public Tank::Application
{
private:
	GLFWwindow *m_window;

	/// <summary>
	/// Root node for editor UI (Hierarchy, Inspector, etc.)
	/// </summary>
	std::shared_ptr<Tank::Node> m_uiRoot;
	std::shared_ptr<SceneView> m_sceneView;
	std::unique_ptr<Framebuffer> m_fb;
public:
	/// <summary>
	/// Callback occurs when window size changes.
	/// </summary>
	static void onWindowSizeChange(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
	
	Editor();
	~Editor();
	void initGL(int w, int h);
	void initImGui();
	void run() override;
	void cleanup();
};
