#pragma once
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
using json = nlohmann::json;


namespace Tank::vec3
{
	json serialise(const glm::vec3 &vec3);
	glm::vec3 deserialise(const json &json);
}
namespace Tank::vec4
{
	json serialise(const glm::vec4 &vec4);
	glm::vec4 deserialise(const json &json);
}
namespace Tank::quat
{
	json serialise(const glm::quat &quat);
	glm::quat deserialise(const json &json);
}
namespace Tank::mat4
{
	json serialise(const glm::mat4 &mat4);
	glm::mat4 deserialise(const json &json);
}