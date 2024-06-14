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
	}

	void Scene::update()
	{
		// Ignore rendering updates if no camera is active.
		if (m_activeCamera == nullptr) return;

		m_root->update();
		m_activeCamera->update();
	}

	void Scene::handleNodeDeletion(Node *deleted) noexcept
	{
		if (m_activeCamera == deleted)
			m_activeCamera = nullptr;
	}
}