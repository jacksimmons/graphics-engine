#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <log.hpp>
#include <file.hpp>
#include <assets/resource.hpp>
#include "user_types.hpp"
#include "sol/sol.hpp"
#include "lua_codegen.hpp"
// The types to-be-defined as usertypes
#include <glm/glm.hpp>
#include <transform.hpp>
#include <key_input.hpp>
#include <nodes/node.hpp>
#include <nodes/camera.hpp>
#include <nodes/scene.hpp>


namespace Tank
{
	std::optional<LuaClass*> UserTypes::classFromName(const std::string &name)
	{
		// First UserType class whose name matches the key
		auto it = std::find_if(s_luaClasses.begin(), s_luaClasses.end(), [&name](const LuaClass &cls) { return name == cls.name; });
		if (it == s_luaClasses.end())
		{
			TE_CORE_ERROR(std::format("Codegen > Tried to find class with name {}, but it didn't exist.", name));
			return {};
		}

		return &(*it);
	}


	void UserTypes::generate(sol::state &lua)
	{
		// Read all usertypes data files
		generateFromFile<glm::vec3>("Vec3", lua);

		UserTypes::Vec3(lua);
		UserTypes::Quat(lua);

		// Define types Node is dependent on
		// Transform
		auto utTransform = lua.new_usertype<Transform>(
			SOL_CLASS("Transform")
		);
		utTransform[SOL_FIELD("Transform", "translation", "Vec3")] = sol::property(&Transform::getLocalTranslation, &Transform::setLocalTranslation);
		utTransform[SOL_FIELD("Transform", "rotation", "Quat")] = sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation);
		utTransform[SOL_FIELD("Transform", "scale", "Vec3")] = sol::property(&Transform::getLocalScale, &Transform::setLocalScale);
		
		// KeyInput, and relevant enums
		lua.new_enum(
			"KeyState",
			"Held", KeyState::Held,
			"Pressed", KeyState::Pressed,
			"NotPressed", KeyState::NotPressed,
			"Released", KeyState::Released
		);
		lua.new_enum(
			"KeyCode",
			KC_PAIR(W),
			KC_PAIR(A),
			KC_PAIR(S),
			KC_PAIR(D)
		);


		// KeyInput
		sol::usertype<KeyInput> utKeyInput = lua.new_usertype<KeyInput>(
			SOL_CLASS("KeyInput")
		);
		utKeyInput[SOL_METHOD("KeyInput", "get_key_state", "KeyState", {{ "code", "KeyCode" }})] = &KeyInput::getKeyState;


		// Node
		sol::usertype<Node> utNode = lua.new_usertype<Node>(
			SOL_CLASS("Node")
		);
		utNode[SOL_FIELD("Node", "name", "string")] = sol::property(&Node::getName, &Node::setName);
		utNode[SOL_FIELD("Node", "transform", "Transform")] = sol::property(&Node::getTransform);
		utNode[SOL_FIELD("Node", "key_input", "KeyInput")] = sol::property(&Node::getKeyInput);
		utNode[SOL_METHOD("Node", "get_parent", "Node", {})] = &Node::getParent;
		utNode[SOL_METHOD("Node", "get_child", "Node", {{ "index", "number" }})] = static_cast<Node *(Node::*)(int) const>(&Node::getChild);
		utNode[SOL_METHOD("Node", "get_child", "Node", {{ "name", "string" }})] = static_cast<Node *(Node::*)(const std::string &) const>(&Node::getChild);
		SOL_GLOBAL_FIELD("Node", "node", "Node");

		// Camera
		sol::usertype<Camera> utCamera = lua.new_usertype<Camera>(
			SOL_CLASS("Camera"),
			sol::base_classes, sol::bases<Node>()
		);
		SOL_CLASS_BASE("Camera", Node);
		utCamera[SOL_FIELD("Camera", "position", "Vec3")] = sol::property(&Camera::getTransformedCentre, &Camera::setPosition);

		// Scene
		sol::usertype<Scene> utScene = lua.new_usertype<Scene>(
			SOL_CLASS("Scene"),
			sol::base_classes, sol::bases<Node>()
		);
		SOL_CLASS_BASE("Scene", Node);
		utScene[SOL_METHOD("Scene", "active_camera", "Camera", {})] = &Scene::getActiveCamera;
		utScene[SOL_STATIC_METHOD("Scene", "current", "Scene", {})] = &Scene::getActiveScene;
	}


	void UserTypes::codegen()
	{
		//if (!File::writeLines(codegenPath.resolvePath(), firstLine))
		//{
		//	TE_CORE_WARN(std::format("Codegen > Failed to clear {}. Cancelling...", codegenPath.resolvePathStr()));
		//	return;
		//}

		fs::create_directories("TankLuaDocs/codegen");
		for (const LuaClass &lc : s_luaClasses)
		{
			Res codegenPath = Res(std::format("TankLuaDocs/codegen/{}.lua", lc.name), true);

			std::ofstream stream;
			// @todo may throw
			stream.open(codegenPath.resolvePathStr(), std::ofstream::out | std::ofstream::trunc);

			stream << "---@meta (GENERATED)\n";
			stream << lc << "\n";
		}

		s_luaClasses.clear();
	}
}