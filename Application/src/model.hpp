#pragma once
#include <glm/glm.hpp>


namespace Tank
{
	class Model
	{
	private:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec4 m_colour;
		float m_scale;
	public:
		Model(glm::vec3 position, glm::vec3 rotation, float scale, glm::vec4 colour);
		~Model();
		void draw();
	};
}