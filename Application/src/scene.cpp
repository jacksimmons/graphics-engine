#include <iostream>
#include "scene.hpp"


namespace Tank
{
	std::shared_ptr<Scene> Scene::s_activeScene = nullptr;


	void Scene::draw() const
	{
		m_root->draw();
	}

	void Scene::update()
	{
		m_root->update();
		m_activeCamera->update();
	}
}