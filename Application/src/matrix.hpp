#pragma once
#include <glm/glm.hpp>


class Matrix
{
public:
	static glm::mat4 rotateAboutPoint(glm::vec3 point, glm::vec3 origin, glm::mat4 rot);
};