#pragma once
#include <functional>
#include <string>
#include "glm/glm.hpp"


namespace Tank
{
	class Widget
	{
	public:
		static void textInput(const char *label, const std::string &hint, std::function<void(const std::string&)> onModified);
		static void vec3Input(const char *label, glm::vec3 current, std::function<void(const glm::vec3&)> onModified);
	};
}