#include <imgui.h>
#include "scene.hpp"
#include "nodes/hierarchy.hpp"
#include "nodes/inspector.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	Hierarchy::Hierarchy(std::string name) : Node(name)
	{
	}

	void Hierarchy::draw() const
	{
		Node *root = Tank::Scene::getActiveScene()->getRoot();

		ImGui::Begin("Hierarchy");
		drawRecursive(root);
		ImGui::End();

		Node::draw();
	}

	void Hierarchy::drawRecursive(Node *node) const
	{
		// Base case.
		if (node == nullptr)
			return;

		Inspector *inspector = (Inspector*)getParent()->getChild("Inspector");
		int childCount = node->getChildCount();

		// Determine if leaf node.
		ImGuiTreeNodeFlags flags = 0;
		if (childCount == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (ImGui::TreeNodeEx(node->getName().c_str(), flags))
		{
			if (ImGui::IsItemFocused())
			{
				inspector->setInspectedNode(node);
			}

			for (int i = 0; i < childCount; i++)
			{
				drawRecursive(node->getChild(i));
			}

			ImGui::TreePop();
		}
	}
}