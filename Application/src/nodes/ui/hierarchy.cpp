#include <vector>
#include <typeinfo>
#include <memory>
#include <imgui.h>
#include "log.hpp"
#include "colours.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"
#include "nodes/scene.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/ui/console.hpp"
#include "nodes/ui/hierarchy.hpp"
#include "nodes/ui/inspector.hpp"


namespace Tank::Editor
{
	_Hierarchy::_Hierarchy(const std::string &name) : _Window(name)
	{
	}


	void _Hierarchy::drawPanel()
	{
		Node *root = Tank::Scene::getActiveScene();

		int count = 0;
		drawTreeNode(root, &count);
	}


	/// <summary>
	/// Generates buttons for all children of the current node, at a given
	/// indentation depth (based on the generation depth).
	/// </summary>
	void _Hierarchy::drawTreeNode(Node *node, int *count) const
	{
		// Base case.
		if (!node) return;

		_Inspector *inspector = (_Inspector*)getSibling("Inspector");
		size_t childCount = node->getChildCount();

		// Determine if leaf node.
		ImGuiTreeNodeFlags flags = 0;
		if (childCount == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;

		// Add node name to the tree, clicking on this node will set `nodeExpanded` to true if not a leaf node.
		ImVec4 nodeNameCol = Colour::NORMAL;
		if (!node->getEnabled()) nodeNameCol = Colour::DISABLED;
		
		ImGui::PushStyleColor(ImGuiCol_Text, nodeNameCol);
		bool nodeExpanded = ImGui::TreeNodeEx((node->getName() + "##" + std::to_string(*count)).c_str(), flags);
		ImGui::PopStyleColor();
		
		// Set the inspected node if necessary: If clicking the non-arrow part of the tree-node, and once the
		// click is complete, and if the tree-node's corresponding node differs from the inspected node.
		if (
			!ImGui::IsItemToggledOpen() &&
			ImGui::IsItemFocused() &&
			!ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
			inspector->getInspectedNode() != node)
		{
			inspector->m_inspectedNode = node;
		}

		// Draw the right-click options, if user is right-clicking and hovering. If node gets deleted here, return.
		if (!drawNodeContextMenu(node, inspector)) goto cleanup;

		// If node was clicked on in the tree, display its children (and further descendants if their parent has previously been expanded).
		if (nodeExpanded)
		{
			// Nodes can be deleted during iteration, so cannot use for-each or iterator syntax.
			for (int i = 0; i < node->getChildCount(); i++)
			{
				drawTreeNode(node->getChild(i), &(++(*count)));
			}
		}

	cleanup:
		if (nodeExpanded)
		{
			// Need to pop tree even if node was deleted.
			ImGui::TreePop();
		}
	}


	bool _Hierarchy::drawNodeContextMenu(Node *node, _Inspector *inspector) const
	{
		bool nodeSurvives = true;

		// If item (node) is hovered and right-clicked...
		if (ImGui::BeginPopupContextItem())
		{
			Node *parent = node->getParent();

			// Render Delete button iff node has a parent (is not root).
			// If Delete is rendered and pressed, this whole statement is true.
			if (parent && ImGui::MenuItem("Delete Node"))
			{
				auto scene = Tank::Scene::getActiveScene();
				scene->onNodeDeleted(node);

				// Handle graceful degradation before node removal.
				inspector->onNodeDeleted(node);

				// Detach child from its parent.
				if (!node->getParent()->removeChild(node))
				{
					std::string line = "Hierarchy: Failed to remove child node from parent.";
					dynamic_cast<_Console*>(getSibling("Console"))->addLine(
						[this, line]()
						{
							ImGui::TextColored(Colour::ERR, line.c_str());
						}
					);
				}

				nodeSurvives = false;
			}

			if (ImGui::BeginMenu("Add Child Node"))
			{
				if (ImGui::MenuItem("Node")) buildNode<Node>(node, "Node");
				if (ImGui::MenuItem("Point Light")) buildNode<PointLight>(node, "PointLight");
				if (ImGui::MenuItem("Directional Light")) buildNode<DirLight>(node, "DirLight");
				if (ImGui::MenuItem("Camera")) buildNode<Camera>(node, "Camera");
				if (ImGui::MenuItem("CubeMap (Skybox)")) buildNode<CubeMap>(node, "CubeMap");

				ImGui::EndMenu();
			}
			
			ImGui::EndPopup();
		}

		return nodeSurvives;
	}


	template <class T>
	Node *_Hierarchy::buildNode(Node *parent, const std::string &name) const
	{
		std::unique_ptr<Node> child = std::unique_ptr<Node>(new T(name));
		Node *ref = child.get();
		parent->addChild(std::move(child));
		return ref;
	}
}