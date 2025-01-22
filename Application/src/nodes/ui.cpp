#include <imgui.h>
#include "nodes/ui.hpp"


namespace Tank
{
	UI::UI(std::string name) : Node(name) {}


	void UI::draw()
	{
		drawUI();
		Node::draw();
	}
}