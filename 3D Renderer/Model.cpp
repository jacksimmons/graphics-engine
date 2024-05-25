#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Model.hpp"


Model::Model(std::unique_ptr<Shape> shape)
{
	m_position = new GLfloat[3] { 0, 0, 0 };
	m_rotation = new GLfloat[3] { 0, 0, 0 };
	m_scale = 1;
	m_colour = new GLfloat[4]{ 0, 0, 0, 1 };

	m_shape = std::move(shape);
}


Model::~Model()
{
	delete m_position;
	delete m_rotation;
	delete m_colour;
}


void Model::draw()
{ 
	m_shape->draw();
}