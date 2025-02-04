#include <stack>
#include <iostream>
#include "log.hpp"
#include "nodes/scene.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	Scene *Scene::s_activeScene = nullptr;


	Scene::Scene(const std::string &name) : Node(name)
	{
		m_activeCamera = nullptr;
	}


	void Scene::update()
	{
		// Ignore rendering updates if no camera is active.
		if (m_activeCamera == nullptr) return;
		m_activeCamera->update();

		Node::update();
	}


	void Scene::addLight(Light *light)
	{
		if (std::find(m_activeLights.begin(), m_activeLights.end(), light) != m_activeLights.end())
		{
			TE_CORE_WARN("Light has already been added to this scene.");
		}
		
		m_activeLights.push_back(light);
	}


	void Scene::removeLight(Light *light)
	{
		auto it = std::find(m_activeLights.begin(), m_activeLights.end(), light);
		if (it != m_activeLights.end())
		{
			m_activeLights.erase(it);
		}
		else
		{
			TE_CORE_WARN("Light was not found in this scene.");
		}
	}


	void Scene::onNodeDeleted(Node *deleted) noexcept
	{
		if (m_activeCamera == deleted)
		{
			m_activeCamera = nullptr;
			return;
		}
	}
}