#include <imgui.h>
#include "hierarchy.hpp"


namespace Tank
{
	Hierarchy::Hierarchy(std::shared_ptr<Scene> scene, std::string name) : UIElement(name)
	{
		m_root = scene->getRoot();
	}


	void Hierarchy::draw() const
	{
		Node::draw();
	}
}