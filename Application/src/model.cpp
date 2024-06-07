#include <glm/glm.hpp>

#include <iostream>
#include <model.hpp>


namespace Tank
{
	Model::Model(glm::vec3 position, glm::vec3 rotation, float scale, glm::vec4 colour)
	{
		m_position = position;
		m_rotation = rotation;
		m_scale = scale;
		m_colour = colour;
	}


	void Model::draw()
	{	}
}