#pragma once
#include <memory>
#include "nodes/ui/window.hpp"


namespace Tank
{
	class Scene; class Model; class Camera; class Light; class DirLight; class PointLight;
	class IShaderContainer;
}

namespace Tank::Editor
{
	class _Inspector final : public _Window
	{
		friend class EditorApp;
		friend class _Hierarchy;
	private:
		Node *m_inspectedNode;

		_Inspector(const std::string &name);
		void drawNodeSection();
		void drawSceneSection(Scene *scene);
		void drawShaderSection(IShaderContainer *shaders);
		void drawCameraSection(Camera *camera);
		void drawLightSection(Light *light);
		void drawDirLightSection(DirLight *dir);
		void onNodeDeleted(Node *node);
		void setInspectedNode(Node *node);
	protected:
		virtual void drawPanel() override;
	public:
		Node *getInspectedNode() const noexcept { return m_inspectedNode; }
	};
}