#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/mat4x4.hpp>


namespace Tank
{
	class Node : public std::enable_shared_from_this<Node>
	{
	private:
		std::string m_name;
		bool m_enabled = true;
	protected:
		std::vector<std::shared_ptr<Node>> m_children;
		std::shared_ptr<Node> m_parent;
		glm::mat4 m_M;

	protected:
		virtual void draw() const;
	public:
		Node(std::string name);
		constexpr void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		constexpr bool getEnabled() const noexcept { return m_enabled; }
		std::string getName() const noexcept { return m_name; }
		std::shared_ptr<Node> getParent() const noexcept { return m_parent; }
		std::shared_ptr<Node> getChild(std::string name) const;
		std::vector<std::shared_ptr<Node>> getChildren() const noexcept { return m_children; }
		void addChild(std::shared_ptr<Node> child);
		glm::mat4 getModelMatrix() const noexcept { return m_M; }
		virtual void update();
	};
}