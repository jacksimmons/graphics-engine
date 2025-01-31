#include "imgui.h"
#include "nodes/ui.hpp"
#include "panel.hpp"


namespace Tank
{
	Panel::Panel(const std::string &name) : UI(name) {}


	void Panel::drawUI()
	{
		ImGui::Begin(getName().c_str());
		drawPanel();
		ImGui::End();
	}
}