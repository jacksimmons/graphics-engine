#include <iostream>
#include "scene.hpp"
#include "nodes/node.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"


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

	void Scene::addLight(Light *light)
	{
		if (std::find(m_activeLights.begin(), m_activeLights.end(), light) == m_activeLights.end())
		{
			m_activeLights.push_back(light);
		}
	}

	void Scene::onNodeDeleted(Node *deleted) noexcept
	{
		if (m_activeCamera == deleted)
		{
			m_activeCamera = nullptr;
			return;
		}
		
		for (int i = 0; i < m_activeLights.size(); i++)
		{
			if (m_activeLights[i] == deleted)
			{
				m_activeLights.erase(m_activeLights.begin() + i);
				return;
			}
		}
	}
}