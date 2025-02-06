#pragma once
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
using json = nlohmann::json;


namespace Tank::vec3
{
	json serialise(const glm::vec3 &vec3);
	glm::vec3 deserialise(const json &json);
}
namespace Tank::quat
{
	json serialise(const glm::quat &quat);
	glm::quat deserialise(const json &json);
}