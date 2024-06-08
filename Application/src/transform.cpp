#include "transform.hpp"


namespace Tank
{
	Transform::Transform()
	{
		m_modelMatrix = glm::identity<glm::mat4>();
		m_scale = { 1,1,1 };
		m_rotation = glm::identity<glm::quat>();
		m_translation = { 1,1,1 };
	}

	void Transform::updateModelMatrix()
	{
		m_modelMatrix =
			  glm::translate(glm::identity<glm::mat4>(), m_translation)
			* glm::scale(glm::identity<glm::mat4>(), m_scale)
			* glm::mat4_cast(m_rotation);
	}
}