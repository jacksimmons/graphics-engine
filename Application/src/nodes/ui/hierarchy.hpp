#pragma once
#include <memory>
#include <type_traits>
#include "nodes/ui/panel.hpp"


namespace Tank::Editor
{
	class _Inspector;
	class _Hierarchy : public _Panel
	{
		friend class EditorApp;
		
		// Fields
	private:
		static std::vector<std::type_info> s_nodeTypes;

		// Methods
	private:
		/// <summary>
		/// Draws a tree node for the node provided, then calls itself for each
		/// of its children. Draws a leaf instead if no children.
		/// </summary>
		void drawTreeNode(Node *node, int *count) const;

		/// <summary>
		/// Handles drawing for node context menus. A node context menu is
		/// displayed when a node is right-clicked in the hierarchy.
		/// This allows the user to Delete Nodes (leading to *node
		/// = nullptr).
		/// Returns true if the node survives, false if it is destroyed.
		/// </summary>
		bool drawNodeContextMenu(Node *node, _Inspector *inspector) const;

		/// <summary>
		/// Creates a new node of the template class, and with the provided node
		/// as a parent.
		/// </summary>
		template <class T>
		Node *buildNode(Node *parent, const std::string &name) const;

		_Hierarchy(const std::string &name);
	protected:
		/// <summary>
		/// Generates buttons for all children of the current node, at a given
		/// indentation depth (based on the generation depth).
		/// </summary>
		virtual void drawPanel() override;
	};
}
