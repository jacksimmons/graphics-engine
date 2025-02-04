#include <imgui.h>
#include "ui.hpp"


namespace Tank
{
	UI::UI(const std::string &name) : Node(name) {}


	void UI::draw()
	{
		drawUI();
		Node::draw();
	}
}