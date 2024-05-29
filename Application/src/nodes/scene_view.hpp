#pragma once
#include <memory>
#include <glad/glad.h>
#include "nodes/ui_element.hpp"
#include "framebuffer.hpp"


class SceneView : public Tank::UIElement
{
	friend class Editor;
private:
	/// <summary>
	/// The viewport size when rendering normally.
	/// </summary>
	int m_sceneW, m_sceneH;

	std::unique_ptr<Framebuffer> m_fb;

	SceneView(glm::ivec2 stdViewportSize, glm::ivec2 fbViewportSize);
public:
	~SceneView();
	void rescale(int w, int h) const;
	void draw() const override;
	constexpr int getSceneW() const noexcept { return m_sceneW; }
	constexpr int getSceneH() const noexcept { return m_sceneH; }
};