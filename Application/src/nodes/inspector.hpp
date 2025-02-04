#pragma once
#include <memory>
#include "nodes/ui/panel.hpp"


namespace Tank
{
	class Scene; class Model; class Camera; class Light; class DirLight;
	class Inspector : public Panel
	{
		friend class Hierarchy;
	private:
		Node *m_inspectedNode;
		bool m_setPropertyValuePanelActive = false;

	private:
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
		Inspector(const std::string &name);

		void setInspectedNode(Node *node) noexcept { m_inspectedNode = node; }
		Node *getInspectedNode() const noexcept { return m_inspectedNode; }
	};
}