#include <vector>
#include <typeinfo>
#include <memory>
#include <imgui.h>
#include "log.hpp"
#include "scene.hpp"
#include "nodes/hierarchy.hpp"
#include "nodes/inspector.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"
#include "nodes/models/cube.hpp"
#include "nodes/models/primitive.hpp"


namespace Tank
{
	Hierarchy::Hierarchy(const std::string &name) : UI(name)
	{
	}


	void Hierarchy::drawUI()
	{
		Node *root = Tank::Scene::getActiveScene()->getRoot();

		ImGui::Begin("Hierarchy");

		int count = 0;
		drawRecursive(root, &count);
		ImGui::End();

		Node::draw();
	}


	/// <summary>
	/// Generates buttons for all children of the current node, at a given
	/// indentation depth (based on the generation depth).
	/// </summary>
	void Hierarchy::drawRecursive(Node *node, int *count) const
	{
		// Base case.
		if (!node) return;

		Inspector *inspector = (Inspector*)getSibling("Inspector");
		int childCount = node->getChildCount();

		// Determine if leaf node.
		ImGuiTreeNodeFlags flags = 0;
		if (childCount == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		bool nodeExpanded = ImGui::TreeNodeEx((node->getName() + "##" + std::to_string(*count)).c_str(), flags);
		
		// `node` can be deleted here.
		drawNodeContextMenu(node, inspector);
		
		if (nodeExpanded)
		{
			// If node was not deleted this frame...
			if (node)
			{
				if (ImGui::IsItemFocused())
				{
					inspector->setInspectedNode(node);
				}
				for (int i = 0; i < childCount; i++)
				{
					drawRecursive(node->getChild(i), &(++(*count)));
				}
			}
			
			// Need to pop tree even if node was deleted.
			ImGui::TreePop();
		}
	}


	void Hierarchy::drawNodeContextMenu(Node *node, Inspector *inspector) const
	{
		// If item (node) is hovered and right-clicked...
		if (ImGui::BeginPopupContextItem())
		{
			Node *parent = node->getParent();

			// Render Delete button iff node has a parent (is not root).
			// If Delete is rendered and pressed, this whole statement is true.
			if (parent && ImGui::MenuItem("Delete Node"))
			{
				// Handle graceful degradation before node removal.
				inspector->onNodeDeleted(node);

				auto scene = Scene::getActiveScene();
				scene->onNodeDeleted(node);

				// Detach child from its parent.
				if (!node->getParent()->removeChild(node))
				{
					TE_CORE_ERROR("Failed to remove child node from parent.");
				}
			}

			if (ImGui::BeginMenu("Add Child Node"))
			{
				if (ImGui::MenuItem("Node")) buildNode<Node>(node, "Node");
				if (ImGui::MenuItem("Cube")) buildNode<Cube>(node, "Cube");
				if (ImGui::MenuItem("Primitive (Line)")) buildNode<Primitive>(node, "Line");
				if (ImGui::MenuItem("Point Light")) buildNode<PointLight>(node, "PointLight");
				if (ImGui::MenuItem("Directional Light")) buildNode<DirLight>(node, "DirLight");
				if (ImGui::MenuItem("Camera")) buildNode<Camera>(node, "Camera");

				ImGui::EndMenu();
			}
			
			ImGui::EndPopup();
		}
	}


	template <class T>
	void Hierarchy::buildNode(Node *parent, const std::string &name) const
	{
		std::unique_ptr<Node> child = std::unique_ptr<Node>(new T(name));
		parent->addChild(std::move(child));
	}
}