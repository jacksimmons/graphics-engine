#include <iostream>
#include <GLFW/glfw3.h>

#include "node.hpp"
#include "transform.hpp"


namespace Tank
{
	Node::Node(std::string name = "Node")
	{
		m_name = name;
		m_transform = std::make_unique<Transform>();
		m_parent = nullptr;
	}

	void Node::addChild(std::unique_ptr<Node> child)
	{
		child->m_parent = this;
		m_children.push_back(std::move(child));
	}

	bool Node::removeChild(Node *child)
	{
		if (child != nullptr)
		{
			for (int i = 0; i < getChildCount(); i++)
			{
				if (m_children[i].get() == child)
				{
					m_children.erase(m_children.begin() + i);
					return true;
				}
			}
		}

		return false;
	}

	Node *Node::getChild(std::string name) const
	{
		for (int i = 0; i < getChildCount(); i++)
		{
			Node *child = getChild(i);
			if (child->getName() == name)
			{
				return child;
			}
		}

		// No child exists by this name.
		return nullptr;
	}

	Node *Node::getChild(int index) const
	{
		if (0 <= index && index < m_children.size())
		{
			return m_children[index].get();
		}
		
		// Out of children list range.
		return nullptr;
	}

	void Node::draw()
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