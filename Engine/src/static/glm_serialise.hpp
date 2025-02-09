#pragma once
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
using json = nlohmann::json;
#include "log.hpp"


namespace Tank
{
	namespace vec3
	{
		json TANK_API serialise(const glm::vec3 &vec3);
		glm::vec3 TANK_API deserialise(const json &json);
	}
	namespace vec4
	{
		json TANK_API serialise(const glm::vec4 &vec4);
		glm::vec4 TANK_API deserialise(const json &json);
	}
	namespace mat4
	{
		json TANK_API serialise(const glm::mat4 &mat4);
		glm::mat4 TANK_API deserialise(const json &json);
	}
	namespace quat
	{
		json TANK_API serialise(const glm::quat &quat);
		glm::quat TANK_API deserialise(const json &json);
	}
}