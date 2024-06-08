#pragma once
#include <memory>
#include "nodes/node.hpp"


namespace Tank
{
	class Inspector;

	class Hierarchy : public Node
	{
		// Methods
	private:
		/// <summary>
		/// Draws a tree node for the node provided, then calls itself for each
		/// of its children. Draws a leaf instead if no children.
		/// </summary>
		void drawRecursive(Node *node) const;

		/// <summary>
		/// Handles drawing for node context menus. A node context menu is
		/// displayed when a node is right-clicked in the hierarchy.
		/// ! This allows the user to Add Nodes, ...
		/// </summary>
		/// <param name="node"></param>
		void drawNodeContextMenu(Node *node, Inspector *inspector) const;
	protected:
		/// <summary>
		/// Generates buttons for all children of the current node, at a given
		/// indentation depth (based on the generation depth).
		/// </summary>
		void draw() const override;
	public:
		Hierarchy(std::string name);
	};
}
