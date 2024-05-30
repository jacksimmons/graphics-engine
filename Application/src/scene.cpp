#include <iostream>
#include "scene.hpp"
#include "nodes/node.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	std::shared_ptr<Scene> Scene::s_activeScene = nullptr;


	Scene::Scene(std::shared_ptr<Node> root, std::shared_ptr<Camera> cam)
	{
		m_root = root;
		m_activeCamera = cam;
	}


	Scene::~Scene()
	{
		m_root = nullptr;
		m_activeCamera = nullptr;
	}


	void Scene::update()
	{
		m_root->update();
		m_activeCamera->update();
	}
}