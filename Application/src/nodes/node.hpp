#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/mat4x4.hpp>

#include "transform.hpp"


namespace Tank
{
	class Node
	{
	private:
		std::string m_name;
		bool m_enabled = true;
	protected:
		Node *m_parent;
		std::vector<std::unique_ptr<Node>> m_children;
		std::unique_ptr<Transform> m_transform;

	// Member Functions
	protected:
		virtual void draw();
	public:
		Node(std::string name);
		
		constexpr void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		constexpr bool getEnabled() const noexcept { return m_enabled; }

		void setName(const std::string &name) noexcept { m_name = name; }
		constexpr const std::string& getName() const noexcept { return m_name; }

		constexpr void setParent(Node *parent) noexcept { m_parent = parent; }
		constexpr Node *getParent() const noexcept { return m_parent; }

		size_t getChildCount() const noexcept { return m_children.size(); }

		// Add an existing child.
		void addChild(std::unique_ptr<Node> child);
		// Removes a child by rawptr, returns boolean of success.
		bool removeChild(Node *child);
		// Get child by name.
		Node *getChild(std::string name) const;
		// Get child by index.
		Node *getChild(int index) const;
		// Get child of parent by name.
		Node *getSibling(std::string name) const { return getParent()->getChild(name); }
		// Get child of parent by index.
		Node *getSibling(int index) const { return getParent()->getChild(index); }

		Transform *getTransform() const { return m_transform.get(); }

		virtual void update();
	};
}