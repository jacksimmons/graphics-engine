#include <iostream>
#include <stack>
#include <algorithm>
#include <GLFW/glfw3.h>

#include "node.hpp"
#include "log.hpp"
#include "transform.hpp"
#include "script.hpp"
#include "static/time.hpp"


namespace Tank
{
	Node::Node(const std::string &name = "Node")
	{
		m_name = name;
		m_transform = std::make_unique<Transform>(this);
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
		for (auto &child : m_children)
		{
			if (child->getName() == name) return child.get();
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

	void Node::forEachDescendant(std::function<void(Node *)> forEach, std::function<bool()> terminate)
	{
		std::stack<Node *> nodeStack;
		nodeStack.push(this);

		while (!nodeStack.empty())
		{
			// Exit early if necessary
			if (terminate && terminate()) return;

			// Pop the top node from the stack, and perform `forEach`.
			Node *node = nodeStack.top();
			nodeStack.pop();
			forEach(node);

			// Add all its children to the stack.
			for (int i = 0; i < node->getChildCount(); i++)
			{
				nodeStack.push(node->getChild(i));
			}
		}
	}

	void Node::addScript(std::unique_ptr<IScript> script)
	{
		m_scripts.push_back(std::move(script));
	}

	bool Node::removeScript(IScript *script)
	{
		for (int i = 0; i < m_scripts.size(); i++)
		{
			if (m_scripts[i].get() == script)
			{
				m_scripts.erase(m_scripts.begin() + i);
				return true;
			}
		}

		return false;
	}


	void Node::draw()
	{
	}

	void Node::update()
	{
		if (!m_enabled) return;
		if (m_visible) draw();

		for (auto const &script : m_scripts)
		{
			script->update();
		}

		for (auto const &child : m_children)
		{
			child->update();
		}
	}
}