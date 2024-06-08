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

		bool nodeExpanded = ImGui::TreeNodeEx(node->getName().c_str(), flags);
		drawNodeContextMenu(node, inspector);
		if (nodeExpanded)
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

	void Hierarchy::drawNodeContextMenu(Node *node, Inspector *inspector) const
	{
		if (ImGui::BeginPopupContextItem())
		{
			Node *parent = node->getParent();
			if (parent && ImGui::MenuItem("Delete Node"))
			{
				// Clear inspector if needed.
				inspector->handleNodeDeletion(node);

				// Detach child from its parent.
				node->getParent()->removeChild(node);
			}
			ImGui::EndPopup();
		}
	}
}