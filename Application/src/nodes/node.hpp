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

	protected:
		virtual void draw() const;
	public:
		Node(std::string name);

		constexpr void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		constexpr bool getEnabled() const noexcept { return m_enabled; }

		void setName(const std::string &name) noexcept { m_name = name; }
		constexpr const std::string& getName() const noexcept { return m_name; }

		constexpr void setParent(Node *parent) noexcept { m_parent = parent; }
		constexpr Node *getParent() const noexcept { return m_parent; }

		size_t getChildCount() const noexcept { return m_children.size(); }
		void addChild(std::unique_ptr<Node> child);
		// Get child by name.
		Node *getChild(std::string name) const;
		// Get child by index.
		Node *getChild(int index) const;

		Transform *getTransform() const { return m_transform.get(); }

		virtual void update();
	};
}