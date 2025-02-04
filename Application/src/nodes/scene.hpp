#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "nodes/node.hpp"


namespace Tank
{
	class Node;
	class Camera;
	class Light;
	class Shader;

	class Scene : public Node
	{
		friend class Hierarchy;
		// Static
	private:
		static Scene *s_activeScene;
	public:
		static Scene *getActiveScene() { return s_activeScene; }

		/// <summary>
		/// Set the active scene.
		/// </summary>
		static void setActiveScene(Scene *scene) { s_activeScene = scene; }

		// Instance
	private:
		Camera *m_activeCamera;
		std::vector<Light *> m_activeLights;

		void onNodeDeleted(Node *deleted) noexcept;
	public:
		// A Scene has ownership of the entire Node hierarchy, and a reference to
		// the active camera.
		Scene(const std::string &name = "Scene");
		
		Camera *getActiveCamera() const noexcept { return m_activeCamera; }
		void setActiveCamera(Camera *camera) noexcept { m_activeCamera = camera; }
		
		void addLight(Light *);
		void removeLight(Light *);
		std::vector<Light *> getActiveLights() const { return m_activeLights; }
		
		virtual void update() override;
	};
}