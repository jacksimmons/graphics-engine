#pragma once
#include <memory>
#include <filesystem>
#include <optional>
#include <unordered_map>
#include <typeinfo>
#include <string>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <core.hpp>
#include "nodes/node.hpp"

using json = nlohmann::json;


namespace Tank
{
	namespace Serialisation
	{
		json serialise(Node *deserialised);
		Node* deserialise(const json &serialised);
	}
}