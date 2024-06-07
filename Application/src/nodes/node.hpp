#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/mat4x4.hpp>


namespace Tank
{
	class Transform;
	class Node : public std::enable_shared_from_this<Node>
	{
	private:
		std::string m_name;
		bool m_enabled = true;

		std::shared_ptr<Transform> m_transform;
	protected:
		std::vector<std::shared_ptr<Node>> m_children;

	protected:
		virtual void draw() const;
	public:
		Node(std::string name);

		constexpr void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		constexpr bool getEnabled() const noexcept { return m_enabled; }

		const std::string& getName() const noexcept { return m_name; }
		
		/// <summary>
		/// Tries to get a child by name.
		/// </summary>
		/// <param name="name">Name of the child.</param>
		/// <param name="outChild">Child output.</param>
		/// <returns>Whether or not this operation succeeded.</returns>
		bool getChild(std::string name, std::shared_ptr<Node> outChild) const;
		void addChild(std::shared_ptr<Node> child);
		const std::vector<std::shared_ptr<Node>> &getChildren() const noexcept { return m_children; }

		Transform& getTransform() const { return *m_transform; }

		virtual void update();
	};
}