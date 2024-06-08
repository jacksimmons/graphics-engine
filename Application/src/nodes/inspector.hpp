#pragma once
#include <memory>
#include "nodes/node.hpp"


namespace Tank
{
	class Inspector : public Node
	{
		// Members
	private:
		Node *m_inspectedNode;


		// Methods
	private:
		void drawInspector() const;
	protected:
		/// <summary>
		/// Generates buttons for all children of the current node, at a given
		/// indentation depth (based on the generation depth).
		/// </summary>
		void draw() const override;
	public:
		Inspector(std::string name);
		void setInspectedNode(Node *node) noexcept { m_inspectedNode = node; }
	};
}