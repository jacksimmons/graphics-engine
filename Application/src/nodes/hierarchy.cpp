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
		
		// `node` can be deleted here, if it is then end the iteration.
		if (!drawNodeContextMenu(node, inspector)) goto cleanup;
		
		if (nodeExpanded)
		{
			// If node was not deleted this frame...
			if (node)
			{
				if (ImGui::IsItemFocused() && inspector->getInspectedNode() != node)
				{
					inspector->setInspectedNode(node);
				}

				// Nodes can be deleted during iteration, so cannot use for-each or iterator syntax.
				for (int i = 0; i < node->getChildCount(); i++)
				{
					drawRecursive(node->getChild(i), &(++(*count)));
				}
			}
		}

	cleanup:
		if (nodeExpanded)
		{
			// Need to pop tree even if node was deleted.
			ImGui::TreePop();
		}
	}


	bool Hierarchy::drawNodeContextMenu(Node *node, Inspector *inspector) const
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
				auto scene = Scene::getActiveScene();
				scene->onNodeDeleted(node);

				// Handle graceful degradation before node removal.
				inspector->onNodeDeleted(node);

				// Detach child from its parent.
				if (!node->getParent()->removeChild(node))
				{
					TE_CORE_ERROR("Failed to remove child node from parent.");
				}

				nodeSurvives = false;
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

		return nodeSurvives;
	}


	template <class T>
	Node *Hierarchy::buildNode(Node *parent, const std::string &name) const
	{
		std::unique_ptr<Node> child = std::unique_ptr<Node>(new T(name));
		Node *ref = child.get();
		parent->addChild(std::move(child));
		return ref;
	}
}