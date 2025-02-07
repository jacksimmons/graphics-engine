#pragma once
#include <memory>
#include "nodes/ui/panel.hpp"


namespace Tank
{
	class Scene; class Model; class Camera; class Light; class DirLight; class PointLight;
}

namespace Tank::Editor
{
	class _Inspector : public _Panel
	{
		friend class EditorApp;

		// The Hierarchy can modify the inspected node, e.g. when a user clicks
		// on one of the nodes.
		friend class _Hierarchy;
	private:
		Node *m_inspectedNode;

	private:
		_Inspector(const std::string &name);
		void drawNodeSection();
		void drawSceneSection(Scene *scene);
		void drawModelSection(Model *model);
		void drawCameraSection(Camera *camera);
		void drawLightSection(Light *light);
		void drawDirLightSection(DirLight *dir);
		void onNodeDeleted(Node *node);
	protected:
		virtual void drawPanel() override;
	public:
		Node *getInspectedNode() const noexcept { return m_inspectedNode; }
	};
}