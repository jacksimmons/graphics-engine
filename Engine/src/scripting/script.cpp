#include <functional>
#include <format>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include "log.hpp"
#include "file.hpp"
#include "json.hpp"
#include "serialisation.hpp"
#include "nodes/node.hpp"
#include "nodes/camera.hpp"
#include "scripting/script.hpp"
#include "scripting/script_prop.hpp"
#include "static/glm_serialise.hpp"


namespace Tank
{
	Script::Script(Node *node, Camera *camera, std::string filename)
		: m_node(node), m_camera(camera), m_filename(filename)
	{
		sol::state *state = new sol::state();
		m_luaState = std::unique_ptr<sol::state>(state);
	}


	Script::~Script()
	{
	}


	std::optional<std::unique_ptr<Script>> Script::createNewScript(Node *node, Camera *camera, const std::string &filename)
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
		
		return std::unique_ptr<Script>(new Script(node, camera, filename));
	}


	std::optional<std::unique_ptr<Script>> Script::createExistingScript(Node *node, Camera *camera, const std::string &filename)
	{
		// Load script (.lua) file
		if (!File::exists(filename))
		{
			TE_CORE_ERROR(std::string("Failed to create from existing script file: ") + filename + " did not exist.");
			return {};
		}

		Script *script = new Script(node, camera, filename);
		return std::unique_ptr<Script>(script);
	}


	void Script::startup()
	{
		// Read script file
		std::string scriptLines;
		std::filesystem::path scriptPath = std::filesystem::path(ROOT_DIRECTORY) / "scripts" / m_filename;
		if (!File::readLines(scriptPath, scriptLines))
		{
			TE_CORE_ERROR(std::string("Script no longer exists: ") + scriptPath.string());
			return;
		}

		// Setup libraries and append /scripts to package.path
		m_luaState->open_libraries(sol::lib::base, sol::lib::package);
		std::string packagePath = (*m_luaState)["package"]["path"];
		packagePath = packagePath + (!packagePath.empty() ? ";" : "") + ROOT_DIRECTORY + "/scripts/?.lua";
		(*m_luaState)["package"]["path"] = packagePath;

		// Setup lua script
		auto result = m_luaState->safe_script(scriptLines, sol::script_pass_on_error);
		if (!result.valid())
		{
			sol::error err = result;
			TE_CORE_ERROR(std::string("Script ") + m_filename + " runtime error: " + err.what());
		}

		// Setup transform property
		Transform *transform = m_node->getTransform();
		(*m_luaState)["_transform"] = ScriptProp::transformToTable(m_luaState.get(), transform);

		// Setup camera property
		(*m_luaState)["_camera"] = sol::table();
		m_luaState->set_function("_camera_translate", [this](float x, float y, float z)
		{
			m_camera->translate({ x,y,z });
		});
		m_luaState->set_function("_camera_rotate", [this](float x, float y, float z)
		{
			m_camera->rotate({ x, y, z });
		});

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

		ScriptProp::tableToTransform((*m_luaState)["_transform"], m_node->getTransform());
	}
}