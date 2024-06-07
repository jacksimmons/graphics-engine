#include "transform.hpp"


namespace Tank
{
	Transform::Transform()
	{
		m_modelMatrix = glm::identity<glm::mat4>();
		m_scale = glm::identity<glm::mat4>();
		m_rotation = glm::identity<glm::quat>();
		m_translation = glm::identity<glm::mat4>();
	}

	void Transform::setRotation(const glm::quat &rot)
	{
		m_rotation = rot;
		updateModelMatrix();
	}

	void Transform::setScale(const glm::vec3 &scale)
	{
		Mat4::setScale(m_scale, scale);
		updateModelMatrix();
	}

	void Transform::setTranslation(const glm::vec3 &trans)
	{
		Mat4::setTranslation(m_translation, trans);
		updateModelMatrix();
	}
}