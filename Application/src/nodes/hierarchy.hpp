#pragma once
#include <memory>
#include "nodes/node.hpp"
#include "nodes/ui_element.hpp"
#include "scene.hpp"


namespace Tank
{
	class Hierarchy : public UIElement
	{
		friend class Editor;
	private:
		std::shared_ptr<Node> m_root;
		Hierarchy(std::shared_ptr<Scene> scene, std::string name);
	protected:
		/// <summary>
		/// Generates buttons for all children of the current node, at a given
		/// indentation depth (based on the generation depth).
		/// </summary>
		void draw() const override;
	};
}