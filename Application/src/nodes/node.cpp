#include <iostream>

#include "node.hpp"
#include "transform.hpp"


namespace Tank
{
	Node::Node(std::string name)
	{
		m_name = name;
		m_transform = std::make_shared<Transform>();
	}

	bool Node::getChild(std::string name, std::shared_ptr<Node> outChild) const
	{
		for (auto const &child : m_children)
		{
			if (child->getName() == name)
			{
				outChild = child;
				return true;
			}
		}

		// If the child was not found, return false;
		return false;
	}

	void Node::addChild(std::shared_ptr<Node> child)
	{
		m_children.push_back(child);
	}

	void Node::draw() const
	{
		for (auto const &child : m_children)
		{
			child->draw();
		}
	}

	void Node::update()
	{
		if (m_enabled)
		{
			draw();
		}
	}
}