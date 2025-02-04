#pragma once
#include <functional>
#include <string>
#include "glm/glm.hpp"


namespace Tank
{
	namespace Widget
	{
		void textInput(const char *label, const std::string &current, std::function<void(const std::string&)> onModified);
		void vec3Input(const char *label, glm::vec3 current, std::function<void(const glm::vec3&)> onModified);
	};
}