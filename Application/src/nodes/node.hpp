#pragma once
#include <vector>
#include <memory>
#include <string>
#include <stack>
#include <glm/mat4x4.hpp>
#include <nlohmann/json.hpp>

#include "transform.hpp"
#include "script.hpp"

using json = nlohmann::json;


namespace Tank
{
	class IScript;

	class Node
	{
	public:
		static json serialise(Node *node);
		static void deserialise(const json &serialised, Node **targetPtr);

	private:
		std::string m_name;
		/// <summary>
		/// If false, Node::update isn't invoked for this Node and all children.
		/// </summary>
		bool m_enabled = true;
		/// <summary>
		/// If false, Node::draw isn't invoked for this Node only.
		/// </summary>
		bool m_visible = true;
	protected:
		std::string m_type;
		Node *m_parent;
		std::unique_ptr<Transform> m_transform;
		std::vector<std::unique_ptr<Node>> m_children;
		std::vector<std::unique_ptr<IScript>> m_scripts;

		// Member Functions
	protected:
		virtual void draw();
	public:
		Node(const std::string &name = "Node");
		virtual ~Node() = default;

		constexpr const std::string& getType() const noexcept { return m_type; }

		constexpr void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		constexpr bool getEnabled() const noexcept { return m_enabled; }

		constexpr void setVisibility(bool visible) noexcept { m_visible = visible; }
		constexpr bool getVisibility() const noexcept { return m_visible; }

		void setName(const std::string &name) noexcept { m_name = name; }
		constexpr const std::string& getName() const noexcept { return m_name; }
		std::string getPath() const;

		constexpr void setParent(Node *parent) noexcept { m_parent = parent; }
		constexpr Node *getParent() const noexcept { return m_parent; }

		Transform *getTransform() const { return m_transform.get(); }

		size_t getChildCount() const noexcept { return m_children.size(); }
		typedef std::vector<std::unique_ptr<Node>>::iterator iterator;
		typedef std::vector<std::unique_ptr<Node>>::const_iterator const_iterator;
		iterator begin() noexcept { return m_children.begin(); }
		iterator end() noexcept { return m_children.end(); }
		const_iterator begin() const noexcept { return m_children.begin(); }
		const_iterator end() const noexcept { return m_children.end(); }

		// Add an existing child.
		void addChild(std::unique_ptr<Node> child);
		// Removes a child by rawptr, returns boolean of success.
		bool removeChild(Node *child);
		// Get first child whose name matches the provided name.
		Node *getChild(std::string name) const;
		// Get child by index.
		Node *getChild(int index) const;
		// Get first sibling whose name matches the provided name.
		Node *getSibling(std::string name) const { return getParent()->getChild(name); }
		// Get sibling by index.
		Node *getSibling(int index) const { return getParent()->getChild(index); }
		// Get parent's child index for this node.
		int getSiblingIndex() const;

		void forEachDescendant(std::function<void(Node *)> forEach, std::function<bool()> terminate = nullptr);

		/// <summary>
		/// Starting at this node, traverse through descendant tree by the instruction of a tree-traversal
		/// array.
		/// 
		/// Element at index n denotes which sibling to continue down, at a depth of n in the descendant tree,
		/// where a depth of 0 is the depth of getChild.
		/// { 1, 0 } = getChild(1)->getChild(0);
		/// </summary>
		Node *childFromTree(std::vector<int> treeTraversal);
		/// <summary>
		/// Builds a descendant tree traversal up to `this`, from a child of `this`.
		/// </summary>
		std::vector<int> treeFromChild(Node *child);

		size_t getScriptCount() const noexcept { return m_scripts.size(); }
		void addScript(std::unique_ptr<IScript> script);
		bool removeScript(IScript *script);
		IScript *getScript(int index) const { return m_scripts[index].get(); }

		virtual void update();
	};
}