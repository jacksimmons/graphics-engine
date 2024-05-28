#include <iostream>
#include "scene.hpp"
#include "nodes/node.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	std::shared_ptr<Scene> Scene::s_activeScene = nullptr;


	void Scene::update()
	{
		m_root->update();
		m_activeCamera->update();
	}
}