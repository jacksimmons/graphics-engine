#pragma once
#include <memory>
#include <filesystem>
#include <optional>
#include <nlohmann/json.hpp>
#include "nodes/scene.hpp"

using json = nlohmann::json;


namespace Tank
{
	namespace Serialisation
	{
		// Load a scene from disk, and gain ownership of it.
		Scene* loadScene(const std::filesystem::path &scenePath);
		void saveScene(Scene *scene, const std::filesystem::path &scenePath);

		json serialise(void *deserialised);
		Node* deserialise(const json &serialised);
	}
}