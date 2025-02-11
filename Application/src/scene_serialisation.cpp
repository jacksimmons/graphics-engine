#include <format>
#include <string>
#include <nlohmann/json.hpp>
#include "scene_serialisation.hpp"
#include "file.hpp"
#include "log.hpp"
#include "nodes/node.hpp"
#include "nodes/camera.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/light.hpp"
#include "nodes/model.hpp"
#include "nodes/scene.hpp"

using json = nlohmann::json;


namespace Tank
{
	namespace Serialisation
	{
		Scene* loadScene(const std::filesystem::path &scenePath)
		{
			std::string sceneFile;
			if (!File::readLines(scenePath, sceneFile))
			{
				TE_CORE_ERROR(std::format("Failed to deserialise from file {}", scenePath.string()));
				return nullptr;
			}

			json serialised = json::parse(sceneFile);
			return dynamic_cast<Scene*>(deserialise(serialised));
		}


		void saveScene(Scene *scene, const std::filesystem::path &scenePath)
		{
			std::string sceneFile;
			if (!File::readLines(scenePath, sceneFile))
			{
				TE_CORE_ERROR(std::format("Failed to serialise to file {}", scenePath.string()));
			}

			// Write with pretty print (indent=4)
			File::writeLines(scenePath, serialise(scene).dump(4));
		}
	}
}