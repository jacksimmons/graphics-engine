#pragma once
#include <functional>
#include "glm/glm.hpp"


namespace Tank
{
	class Widget
	{
	public:
		static void vec3Input(const char *label, glm::vec3 current, std::function<void(glm::vec3)> onModified);
	};
}