#pragma once
#include <memory>
#include "nodes/ui_element.hpp"


namespace Tank
{
	class Node;
	class Scene;
}


class Hierarchy : public Tank::UIElement
{
	friend class Editor;
private:
	Hierarchy(std::string name) : UIElement(name) {};

	/// <summary>
	/// Draws a tree node for the node provided, then calls itself for each
	/// of its children. Draws a leaf instead if no children.
	/// </summary>
	void drawRecursive(std::shared_ptr<Node> node) const;
protected:
	/// <summary>
	/// Generates buttons for all children of the current node, at a given
	/// indentation depth (based on the generation depth).
	/// </summary>
	void draw() const override;
};
