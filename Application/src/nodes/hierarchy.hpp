#pragma once
#include <memory>
#include <type_traits>
#include "nodes/ui.hpp"


namespace Tank
{
	class Inspector;
	class Node;
	class Hierarchy : public UI
	{
		// Fields
	private:
		static std::vector<std::type_info> s_nodeTypes;

		// Methods
	private:
		/// <summary>
		/// Draws a tree node for the node provided, then calls itself for each
		/// of its children. Draws a leaf instead if no children.
		/// </summary>
		void drawRecursive(Node *node, int *count) const;

		/// <summary>
		/// Handles drawing for node context menus. A node context menu is
		/// displayed when a node is right-clicked in the hierarchy.
		/// This allows the user to Delete Nodes (leading to *node
		/// = nullptr).
		/// </summary>
		/// <param name="node"></param>
		void drawNodeContextMenu(Node **node, Inspector *inspector) const;

		/// <summary>
		/// Creates a new node of the template class, and with the provided node
		/// as a parent.
		/// </summary>
		template <class T>
		void buildNode(Node *parent, const std::string &name) const;
	protected:
		/// <summary>
		/// Generates buttons for all children of the current node, at a given
		/// indentation depth (based on the generation depth).
		/// </summary>
		void drawUI() override;
	public:
		Hierarchy(const std::string &name);
	};
}
