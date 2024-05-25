#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <algorithm>

#include "Scene.hpp"
#include "Error.hpp"


Scene::Scene(int width, int height)
{
	running = true;
	this->width = width;
	this->height = height;
}


void Scene::addModel(std::unique_ptr<Model> model)
{
	m_models.push_back(std::move(model));
}


void Scene::draw()
{
	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i]->draw();
	}
}