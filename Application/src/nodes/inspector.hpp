#pragma once
#include <memory>
#include "nodes/node.hpp"


namespace Tank
{
	class Model; class Camera; class Light;
	class Inspector : public Node
	{
		friend class Hierarchy;
	private:
		Node *m_inspectedNode;

	private:
		void drawNodeSection() const;
		void drawModelSection(Model *model) const;
		void drawCameraSection(Camera *camera) const;
		void drawLightSection(Light *light) const;
		void onNodeDeleted(Node *node);
	protected:
		void draw() const override;
	public:
		Inspector(std::string name);

		void setInspectedNode(Node *node) noexcept { m_inspectedNode = node; }
	};
}