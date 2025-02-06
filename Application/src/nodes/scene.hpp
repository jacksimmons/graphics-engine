#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>
#include "nodes/node.hpp"
#include "nodes/camera.hpp"

using json = nlohmann::json;


namespace Tank
{
	class Node;
	class Camera;
	class Light;
	class Shader;

	class Scene : public Node
	{
	public:
		static json serialise(Scene *scene);
		static void deserialise(const json &serialised, Scene **targetPtr);

		// Static
	private:
		friend class Hierarchy;
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
		std::vector<Light *> m_lights;
		unsigned m_numDirLights = 0;
		unsigned m_numPointLights = 0;

		void onNodeDeleted(Node *deleted) noexcept;
	public:
		// A Scene has ownership of the entire Node hierarchy, and a reference to
		// the active camera.
		Scene(const std::string &name = "Scene");
		
		// Get the active camera for this scene.
		Camera *getActiveCamera() const noexcept { return m_activeCamera; }
		// Set the active camera for this scene.
		void setActiveCamera(Camera *camera) noexcept { m_activeCamera = camera; }
		
		// Adds a light to the scene. Returns the light's index.
		unsigned addLight(Light *);
		// Removes a light to the scene.
		void removeLight(Light *);

		std::vector<Light *> getLights() const { return m_lights; }
		unsigned getNumDirLights() const { return m_numDirLights; }
		unsigned getNumPointLights() const { return m_numPointLights; }
		
		virtual void update() override;
	};
}