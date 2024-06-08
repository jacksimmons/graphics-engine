#pragma once
#include <vector>
#include <memory>
#include <map>


namespace Tank
{
	class Node;
	class Camera;
	class Scene
	{
		// Static
	private:
		static Scene *s_activeScene;
	public:
		static Scene *getActiveScene() { return s_activeScene; }

		/// <summary>
		/// Set the active scene, if one has not been set already.
		/// </summary>
		static void setActiveScene(Scene *scene)
		{
			if (s_activeScene == nullptr && scene != nullptr)
				s_activeScene = scene;
		}

		// Instance
	private:
		std::unique_ptr<Node> m_root;
		Camera *m_activeCamera;
	public:
		// A Scene has ownership of the entire Node hierarchy, and a reference to
		// the active camera.
		Scene(std::unique_ptr<Node> root, Camera *cam);
		~Scene();
		Camera *getActiveCamera() const noexcept { return m_activeCamera; }
		Node *getRoot() const noexcept { return m_root.get(); }	
		void update();
	};
}