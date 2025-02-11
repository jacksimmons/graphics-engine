#pragma once
#include <memory>
#include <filesystem>
#include <optional>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <core.hpp>
#include "nodes/node.hpp"

using json = nlohmann::json;


namespace Tank
{
	namespace Serialisation
	{
		TANK_API json serialise(Node *deserialised);
		TANK_API Node* deserialise(const json &serialised);
	}
}