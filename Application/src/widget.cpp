#include <functional>
#include "imgui.h"
#include "widget.hpp"


namespace Tank
{
	void Widget::vec3Input(const char *label, glm::vec3 current, std::function<void(glm::vec3)> onModified)
	{
		float currentArr[] = { current.x, current.y, current.z };
		if (ImGui::InputFloat3(label, currentArr))
		{
			glm::vec3 modified = { currentArr[0], currentArr[1], currentArr[2] };
			onModified(modified);
		}
	}
}