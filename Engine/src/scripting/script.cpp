#include <functional>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <variant>
#include "log.hpp"
#include "file.hpp"
#include "json.hpp"
#include "serialisation.hpp"
#include "nodes/node.hpp"
#include "scripting/script.hpp"
#include "static/glm_serialise.hpp"


namespace Tank
{
	Script::Script(Node *node, std::string filename)
		: m_node(node), m_filename(filename)
	{
		sol::state *state = new sol::state();
		m_luaState = std::unique_ptr<sol::state>(state);
	}


	Script::~Script()
	{
	}


	std::optional<std::unique_ptr<Script>> Script::createNewScript(Node *node, const std::string &filename)
	{
		std::string scriptsDir = std::string(ROOT_DIRECTORY) + "/scripts";
		std::string scriptPath = scriptsDir + "/" + filename;

		// Copy the template
		std::string newScriptLines;
		if (!File::readLines(scriptsDir + "/template.lua", newScriptLines))
		{
			TE_CORE_ERROR("Failed to read template script.");
			return {};
		}

		// Check if the filename provided exists
		if (File::exists(scriptPath))
		{
			TE_CORE_INFO(std::string("Script file already exists: ") + scriptPath);
		}
		// Only copy the template into this new file if it doesn't already exist
		else
		{
			// Write the copied template into new file
			if (!File::writeLines(scriptPath, newScriptLines))
			{
				TE_CORE_ERROR("Failed to create new script.");
				return {};
			}
		}
		
		return std::unique_ptr<Script>(new Script(node, filename));
	}


	std::optional<std::unique_ptr<Script>> Script::createExistingScript(Node *node, const std::string &filename)
	{
		// Load script (.lua) file
		if (!File::exists(filename))
		{
			TE_CORE_ERROR(std::string("Failed to create from existing script file: ") + filename + " did not exist.");
			return {};
		}

		Script *script = new Script(node, filename);
		return std::unique_ptr<Script>(script);
	}


	void Script::startup()
	{
		// Read script file
		std::string scriptLines;
		std::filesystem::path scriptPath = std::filesystem::path(ROOT_DIRECTORY) / "Scripts" / m_filename;
		if (!File::readLines(scriptPath, scriptLines))
		{
			TE_CORE_ERROR(std::string("Script no longer exists: ") + scriptPath.string());
			return;
		}

		// Setup lua script
		m_luaState->open_libraries(sol::lib::base, sol::lib::package);
		auto result = m_luaState->safe_script(scriptLines, sol::script_pass_on_error);
		if (!result.valid())
		{
			sol::error err = result;
			TE_CORE_ERROR(std::string("Script ") + m_filename + " runtime error: " + err.what());
		}

		// Setup script functions
		Transform *transform = m_node->getTransform();
		const auto &trans = transform->getLocalTranslation();
		const auto &rot = glm::eulerAngles(transform->getLocalRotation());
		const auto &scale = transform->getLocalScale();

		(*m_luaState)["transform"] = m_luaState->create_table_with(
			"translation", m_luaState->create_table_with("x", trans.x, "y", trans.y, "z", trans.z),
			"rotation", m_luaState->create_table_with("x", rot.x, "y", rot.y, "z", rot.z),
			"scale", m_luaState->create_table_with("x", scale.x, "y", scale.y, "z", scale.z)
		);

		// Call startup function on lua script, if present
		std::optional<sol::protected_function> start = (*m_luaState)["Startup"];
		if (start.has_value())
			start.value()();
	}


	void Script::shutdown()
	{
		// Call shutdown function on lua script, if present
		std::optional<sol::protected_function> shutdown = (*m_luaState)["Shutdown"];
		if (shutdown.has_value())
			shutdown.value()();
	}


	void Script::update()
	{
		if (!m_enabled) return;

		// Call update function on lua script, if present
		std::optional<sol::protected_function> update = (*m_luaState)["Update"];
		if (update.has_value())
			update.value()();

		Transform *transform = m_node->getTransform();
		auto luaTransform = (*m_luaState)["transform"];
		transform->setLocalTranslation({ luaTransform["translation"]["x"], luaTransform["translation"]["y"], luaTransform["translation"]["z"] });
		transform->setLocalRotation(quat::fromAngleAxis({ luaTransform["rotation"]["x"], luaTransform["rotation"]["y"], luaTransform["rotation"]["z"] }));
		transform->setLocalScale({ luaTransform["scale"]["x"], luaTransform["scale"]["y"], luaTransform["scale"]["z"] });
	}
}