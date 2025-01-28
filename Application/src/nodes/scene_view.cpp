#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.hpp"
#include "scene.hpp"
#include "framebuffer.hpp"
#include "key_input.hpp"
#include "nodes/scene_view.hpp"


namespace Tank
{
	SceneView::SceneView(const std::string &name, glm::ivec2 sceneViewportSize, glm::ivec2 fbViewportSize, KeyInput *keyInput) : UI(name)
	{
		m_sceneW = sceneViewportSize.x, m_sceneH = sceneViewportSize.y;
		m_fb = std::make_unique<Framebuffer>(fbViewportSize.x, fbViewportSize.y);
		m_keyInput = keyInput;
		m_isFocussed = false;
	}


	void SceneView::drawUI()
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Just sets default panel-window size.
		ImGui::SetNextWindowSize(ImVec2(fbW + 10.0f, fbH + 10.0f), ImGuiCond_FirstUseEver);

		ImGui::Begin("SceneView");
		{
			ImGui::BeginChild("SceneRender");
			m_isFocussed = ImGui::IsWindowFocused();

			ImVec2 wsize = ImGui::GetWindowSize();
			int fbWNew = wsize.x - 10;
			int fbHNew = wsize.y - 10;

			if (fbWNew != fbW || fbHNew != fbH)
				rescale(fbWNew, fbHNew);

			ImVec2 fbsize = ImVec2((float)fbW, (float)fbH);
			ImTextureID imTex = (ImTextureID)(intptr_t)m_fb->getTexColBuf();

			ImGui::Image(imTex, fbsize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			ImGui::EndChild();
		}
		ImGui::End();
	}


	void SceneView::update(float frameDelta)
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Set viewport to fbo, render into fbo, set viewport to std.
		glViewport(0, 0, fbW, fbH);
		m_fb->update(frameDelta);
		glViewport(0, 0, m_sceneW, m_sceneH);

		UI::update(frameDelta);
	}


	void SceneView::rescale(int w, int h) const
	{
		m_fb->rescale(w, h);
	}

	
	void SceneView::handleKeyInput() const
	{
		if (!m_isFocussed) return;

		auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
		if (cam == nullptr) return;
		float panSpd = cam->getPanSpeed();
		float rotSpd = cam->getRotSpeed();

		if (m_keyInput->getKeyState(GLFW_KEY_F1) == KeyState::Pressed)
			m_keyInput->cycleRenderMode();


		if (m_keyInput->getKeyState(GLFW_KEY_W) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, panSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_A) == KeyState::Held)
			cam->translate(glm::vec3(-panSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_S) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, -panSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_D) == KeyState::Held)
			cam->translate(glm::vec3(panSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_Q) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, 0.0f, panSpd));

		if (m_keyInput->getKeyState(GLFW_KEY_E) == KeyState::Held)
			cam->translate(glm::vec3(0.0f, 0.0f, -panSpd));


		if (m_keyInput->getKeyState(GLFW_KEY_J) == KeyState::Held)
			cam->rotate(glm::vec3(-rotSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_L) == KeyState::Held)
			cam->rotate(glm::vec3(rotSpd, 0.0f, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_I) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, rotSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_K) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, -rotSpd, 0.0f));

		if (m_keyInput->getKeyState(GLFW_KEY_U) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, 0.0f, rotSpd));

		if (m_keyInput->getKeyState(GLFW_KEY_O) == KeyState::Held)
			cam->rotate(glm::vec3(0.0f, 0.0f, -rotSpd));
	}
}