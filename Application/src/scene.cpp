#include <iostream>
#include "scene.hpp"
#include "nodes/node.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	Scene *Scene::s_activeScene = nullptr;


	Scene::Scene(std::unique_ptr<Node> root, Camera *cam)
	{
		m_root = std::move(root);
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