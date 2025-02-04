#include <format>
#include <functional>
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "log.hpp"
#include "scene.hpp"
#include "framebuffer.hpp"
#include "key_input.hpp"
#include "static/time.hpp"
#include "nodes/scene_view.hpp"
#include "nodes/ui/console.hpp"
#include "colours.hpp"


namespace Tank
{
	SceneView::SceneView(const std::string &name, glm::ivec2 sceneViewportSize, glm::ivec2 fbViewportSize, KeyInput *keyInput) : Panel(name)
	{
		m_sceneW = sceneViewportSize.x, m_sceneH = sceneViewportSize.y;
		m_fb = std::make_unique<Framebuffer>(fbViewportSize.x, fbViewportSize.y);
		m_keyInput = keyInput;
		m_isFocussed = false;
		m_polygonMode = GL_FILL;
	}


	void SceneView::drawUI()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Just sets default panel-window size.
		ImGui::SetNextWindowSize(ImVec2(fbW + 10.0f, fbH + 10.0f), ImGuiCond_FirstUseEver);
		Panel::drawUI();
	}


	void SceneView::drawPanel()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		ImGui::BeginChild("SceneRender");
		m_isFocussed = ImGui::IsWindowFocused();

		ImVec2 wsize = ImGui::GetWindowSize();
		int fbWNew = (int)wsize.x - 10;
		int fbHNew = (int)wsize.y - 10;

		if (fbWNew != fbW || fbHNew != fbH)
			rescale(fbWNew, fbHNew);

		ImVec2 fbsize = ImVec2((float)fbW, (float)fbH);
		ImTextureID imTex = (ImTextureID)(intptr_t)m_fb->getTexColBuf();

		// Update FPS counter at a constant frequency. If not updated,
		// display previous FPS value.
		float delta = Time::getFrameDelta();
		m_fpsDisplayUpdateTimer += delta;
		if (m_fpsDisplayUpdateTimer > FPS_DISPLAY_UPDATE_FREQUENCY)
		{
			m_fpsDisplayLastText = std::format("{} FPS", 1 / delta).c_str();
			m_fpsDisplayUpdateTimer = 0;
		}
		ImGui::Text(m_fpsDisplayLastText.c_str());

		ImGui::Image(imTex, fbsize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::EndChild();
	}


	void SceneView::update()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Set viewport to fbo, render into fbo, set viewport to std.
		glViewport(0, 0, fbW, fbH);
		m_fb->update();
		glViewport(0, 0, m_sceneW, m_sceneH);

		UI::update();
	}


	void SceneView::rescale(int w, int h) const
	{
		m_fb->rescale(w, h);
	}

	
	void SceneView::handleKeyInput()
	{
		if (!m_isFocussed) return;

		auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
		if (cam == nullptr) return;
		float panSpd = cam->getPanSpeed();
		float rotSpd = cam->getRotSpeed();

		if (m_keyInput->getKeyState(GLFW_KEY_F1) == KeyState::Pressed)
			cyclePolygonMode();

		float frameDelta = Time::getFrameDelta();

		if (m_keyInput->getKeyState(GLFW_KEY_W) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, frameDelta * panSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_A) == KeyState::Held)
			cam->translate(glm::vec3(-frameDelta * panSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_S) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, -frameDelta * panSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_D) == KeyState::Held)
			cam->translate(glm::vec3(frameDelta * panSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_Q) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, 0.0f, frameDelta * panSpd));

		if (m_keyInput->getKeyState(GLFW_KEY_E) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, 0.0f, -frameDelta * panSpd));


		if (m_keyInput->getKeyState(GLFW_KEY_J) == KeyState::Held)
			cam->rotate(glm::vec3(-frameDelta * rotSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_L) == KeyState::Held)
			cam->rotate(glm::vec3(frameDelta * rotSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_I) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, frameDelta * rotSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_K) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, -frameDelta * rotSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_U) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, 0.0f, frameDelta * rotSpd));

		if (m_keyInput->getKeyState(GLFW_KEY_O) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, 0.0f, -frameDelta * rotSpd));
	}


	void SceneView::cyclePolygonMode()
	{
		switch (m_polygonMode)
		{
		case GL_FILL:
			m_polygonMode = GL_POINT;
			break;
		case GL_POINT:
			m_polygonMode = GL_LINE;
			break;
		case GL_LINE:
			m_polygonMode = GL_FILL;
			break;
		}

		glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
		std::string line = std::format("Set GL polygon mode to {}",
			m_polygonMode == GL_FILL ? "FILL" : (m_polygonMode == GL_POINT ? "POINT" : "LINE"));

		dynamic_cast<Console*>(getSibling("Console"))->addLine(
			[this, line]()
			{
				ImGui::TextColored(Colour::ERR, line.c_str());
			}
		);
	}
}