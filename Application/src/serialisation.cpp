#include <format>
#include <string>
#include <nlohmann/json.hpp>
#include "serialisation.hpp"
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
	Scene* Serialisation::loadScene(const std::filesystem::path &scenePath)
	{
		std::string sceneFile;
		if (!File::readLines(scenePath, sceneFile))
		{
			TE_CORE_ERROR(std::format("Failed to deserialise from file {}", scenePath.string()));
			return nullptr;
		}

		json serialised = json::parse(sceneFile);
		return dynamic_cast<Scene*>( deserialise(serialised) );
	}


	void Serialisation::saveScene(Scene *scene, const std::filesystem::path &scenePath)
	{
		std::string sceneFile;
		if (!File::readLines(scenePath, sceneFile))
		{
			TE_CORE_ERROR(std::format("Failed to serialise to file {}", scenePath.string()));
		}

		// Write with pretty print (indent=4)
		File::writeLines(scenePath, serialise(scene).dump(4));
	}


	json Serialisation::serialise(void *deserialised)
	{
		json serialised;
		const std::string &type = ((Node*)deserialised)->getType();

		if (type == "Node") serialised = Node::serialise((Node*)deserialised);
		else if (type == "Camera") serialised = Camera::serialise((Camera*)deserialised);
		else if (type == "Skybox") serialised = Skybox::serialise((Node*)deserialised);
		else if (type == "DirLight") serialised = DirLight::serialise((DirLight*)deserialised);
		else if (type == "PointLight") serialised = PointLight::serialise((Light*)deserialised);
		else if (type == "Model") serialised = Model::serialise((Model*)deserialised);
		else serialised = Scene::serialise((Scene*)deserialised);

		std::vector<json> children;
		for (auto &child : *(Node*)deserialised)
		{
			children.push_back(serialise(child.get()));
		}
		serialised["children"] = children;

		return serialised;
	}


	Node* Serialisation::deserialise(const json &serialised)
	{
		// Pointer to a Node* or subclass of. 
		Node *node = nullptr;
		const std::string &type = serialised["type"];

		// +Node
		if (type == "Node") Node::deserialise(serialised, &node);
		else if (type == "Camera") Camera::deserialise(serialised, (Camera**)&node);
		else if (type == "Skybox") Skybox::deserialise(serialised, &node);
		else if (type == "DirLight") DirLight::deserialise(serialised, (DirLight**)&node);
		else if (type == "PointLight") PointLight::deserialise(serialised, (Light**)&node);
		else if (type == "Model") Model::deserialise(serialised, (Model**)&node);
		else Scene::deserialise(serialised, (Scene**)&node);

		for (const json &child : serialised["children"].get<std::vector<json>>())
		{
			node->addChild(std::unique_ptr<Node>(deserialise(child)));
		}

		// Post-tree instantiation (after all children have been deserialised)
		if (type == "Scene")
		{
			Scene *scene = (Scene*)node;
			scene->setActiveCamera((Camera*)scene->childFromTree(serialised["activeCam"]));
		}

		return node;
	}
}