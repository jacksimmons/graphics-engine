#pragma once
#include <optional>
#include <memory>
#include <string>
#include <core.hpp>


namespace sol
{
	class state;
}
namespace Tank
{
	class Transform;
	class Node;
	class Camera;

	/// <summary>
	/// A script which can be attached to a Node subclass.
	/// Binds to a lua script file, which can in turn access some properties
	/// of the Script object, such as its node.
	/// </summary>
	class TANK_API Script final
	{
	private:
		bool m_enabled = true;
		Node *m_node;
		Camera *m_camera;
		std::string m_filename;
		std::unique_ptr<sol::state> m_luaState;
		Script(Node *node, Camera *camera, std::string filename);
	public:
		~Script();

		static std::optional<std::unique_ptr<Script>> createNewScript(Node *node, Camera *camera, const std::string &filename);
		static std::optional<std::unique_ptr<Script>> createExistingScript(Node *node, Camera *camera, const std::string &filename);

		void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		bool getEnabled() const noexcept { return m_enabled; }

		const std::string &getFilename() { return m_filename; }

		void startup();
		void shutdown();
		void update();
	};
}