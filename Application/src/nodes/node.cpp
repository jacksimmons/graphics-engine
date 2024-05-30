#include <iostream>
#include "node.hpp"


namespace Tank
{
	Node::Node(std::string name)
	{
		m_M = glm::mat4(1.0f);
		m_name = name;
		m_parent = nullptr;
	}

	std::shared_ptr<Node> Node::getChild(std::string name) const
	{
		for (auto const &child : m_children)
		{
			if (child->getName() == name)
			{
				return child;
			}
		}

		// Return nullptr if no child was found.
		return std::shared_ptr<Node>();
	}

	void Node::addChild(std::shared_ptr<Node> child)
	{
		child->m_parent = shared_from_this();
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