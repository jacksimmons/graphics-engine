#include <imgui.h>
#include <glad/glad.h>
#include "log.hpp"
#include "scene.hpp"
#include "nodes/scene_view.hpp"


namespace Tank
{
	SceneView::SceneView(std::string name, glm::ivec2 sceneViewportSize, glm::ivec2 fbViewportSize) : Node(name)
	{
		m_sceneW = sceneViewportSize.x, m_sceneH = sceneViewportSize.y;
		m_fb = std::make_unique<Framebuffer>(fbViewportSize.x, fbViewportSize.y);
	}

	SceneView::~SceneView()
	{
	}

	void SceneView::rescale(int w, int h) const
	{
		m_fb->rescale(w, h);
	}

	void SceneView::draw() const
	{
		int fbW = m_fb->getW(), fbH = m_fb->getH();

		// Set viewport to fbo, render into fbo, set viewport to std.
		glViewport(0, 0, fbW, fbH);
		m_fb->update();
		glViewport(0, 0, m_sceneW, m_sceneH);

		// Just sets default panel-window size.
		ImGui::SetNextWindowSize(ImVec2(fbW + 10, fbH + 10), ImGuiCond_FirstUseEver);

		ImGui::Begin("SceneView");
		{
			ImGui::BeginChild("SceneRender");

			ImVec2 wsize = ImGui::GetWindowSize();
			rescale(wsize.x - 10, wsize.y - 10);

			ImGui::Image((ImTextureID)m_fb->getTexColBuf(), ImVec2(fbW, fbH), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}
		ImGui::End();
	}
}