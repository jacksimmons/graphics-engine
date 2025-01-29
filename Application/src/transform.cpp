#include "transform.hpp"
#include "nodes/node.hpp"


namespace Tank
{
	Transform::Transform(Node *owner)
	{
		m_modelMatrix = glm::identity<glm::mat4>();
		m_scale = { 1,1,1 };
		m_rotation = glm::identity<glm::quat>();
		m_translation = { 0,0,0 };
		m_owner = owner;
	}


	glm::mat4 Transform::getModelMatrix() const
	{
		return (glm::translate(glm::identity<glm::mat4>(), m_translation)
			* glm::scale(glm::identity<glm::mat4>(), m_scale)
			* glm::mat4_cast(m_rotation));
	}


	glm::mat4 Transform::getWorldMatrix() const
	{
		if (m_owner->getParent())
		{
			return m_owner->getParent()->getTransform()->getWorldMatrix() * getModelMatrix();
		}

		return getModelMatrix();
	}
}