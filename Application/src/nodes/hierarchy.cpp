#include <imgui.h>
#include "matrix.hpp"
#include "scene.hpp"
#include "nodes/hierarchy.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	Hierarchy::Hierarchy(std::string name) : UIElement(name)
	{
	}

	void Hierarchy::draw() const
	{
		std::shared_ptr<Node> root = Tank::Scene::getActiveScene()->getRoot();

		ImGui::Begin("Hierarchy");
		drawRecursive(root);
		ImGui::End();

		Node::draw();
	}

	void Hierarchy::drawRecursive(std::shared_ptr<Node> node) const
	{
		auto children = node->getChildren();

		// Determine if leaf node.
		ImGuiTreeNodeFlags flags = 0;
		if (children.size() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (ImGui::TreeNodeEx(node->getName().c_str(), flags))
		{
			if (ImGui::IsItemFocused())
			{
				auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
				cam->setPosition(Matrix::getPosition(node->getModelMatrix()));
				cam->setRotation(glm::vec3());
			}

			for (std::shared_ptr<Node> child : children)
			{
				drawRecursive(child);
			}

			ImGui::TreePop();
		}
	}
}