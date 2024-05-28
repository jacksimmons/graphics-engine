#pragma once
#include <vector>
#include <memory>
#include <string>


namespace Tank
{
	class Node
	{
		friend class Scene;
	private:
		bool m_enabled = true;
		std::string m_name;
	protected:
		std::vector<std::shared_ptr<Node>> m_children;
		std::shared_ptr<Node> m_parent;

	protected:
		virtual void draw() const;
	public:
		static void addChild(std::shared_ptr<Node> parent, std::shared_ptr<Node> child);

		Node(std::string name);
		constexpr void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		constexpr bool getEnabled() const noexcept { return m_enabled; }
		std::string getName() const noexcept { return m_name; }
		std::shared_ptr<Node> getChild(std::string name) const;
		std::vector<std::shared_ptr<Node>> getChildren() const noexcept { return m_children; }
		virtual void update();
	};
}