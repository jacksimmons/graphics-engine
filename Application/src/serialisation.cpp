#include <format>
#include <string>
#include <nlohmann/json.hpp>
#include "serialisation.hpp"
#include "file.hpp"
#include "log.hpp"

using json = nlohmann::json;


namespace Tank
{
	Scene* Serialisation::loadScene(const std::filesystem::path &scenePath)
	{
		std::string sceneFile;
		if (!File::readLines(scenePath, sceneFile))
		{
			TE_CORE_ERROR(std::format("Failed to deserialise from file {}", scenePath.string()));
			return nullptr;
		}

		json serialised = json::parse(sceneFile);
		return dynamic_cast<Scene*>( deserialise<Scene>(serialised) );
	}

	
	template <class T>
	Node* Serialisation::deserialise(json serialised)
	{
		Node *node = dynamic_cast<Node*>(new T(serialised["name"]));
		node->setName(serialised["name"]);
		node->setEnabled(serialised["enabled"]);
		node->setVisibility(serialised["visible"]);

		for (auto &serialisedChild : serialised["children"])
		{
			node->addChild(std::unique_ptr<Node>(deserialise<Node>(serialisedChild)));
		}

		return node;
	}


	void Serialisation::saveScene(Scene *scene, const std::filesystem::path &scenePath)
	{
		std::string sceneFile;
		if (!File::readLines(scenePath, sceneFile))
		{
			TE_CORE_ERROR(std::format("Failed to serialise to file {}", scenePath.string()));
		}

		// Write with pretty print (indent=4)
		File::writeLines(scenePath, scene->serialise().dump(4));
	}
}