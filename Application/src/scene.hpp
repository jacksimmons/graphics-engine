#pragma once
#include <vector>
#include <memory>

#include "nodes/node.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	class Scene
	{
		// Static
	private:
		static std::shared_ptr<Scene> s_activeScene;
	public:
		static std::shared_ptr<Scene> getActiveScene() { return s_activeScene; }
		/// <summary>
		/// Set the active scene, if one has not been set already.
		/// </summary>
		static void setActiveScene(std::shared_ptr<Scene> scene)
		{
			if (s_activeScene == nullptr && scene != nullptr)
				s_activeScene = scene;
		}

		// Instance
	private:
		std::shared_ptr<Node> m_root;
		std::shared_ptr<Camera> m_activeCamera;
	public:

		Scene(std::shared_ptr<Node> root, std::shared_ptr<Camera> cam) : m_root(root), m_activeCamera(std::move(cam)) {};
		std::shared_ptr<Camera> getActiveCamera() const noexcept { return m_activeCamera; }
		std::shared_ptr<Node> getRoot() const noexcept { return m_root; }
		void draw() const;
		void update();
	};
}