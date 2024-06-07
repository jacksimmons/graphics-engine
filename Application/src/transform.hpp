#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "transformation.hpp"


namespace Tank
{
	class Transform
	{
	private:
		// Entire transformation
		glm::mat4 m_modelMatrix;

		// Individual transformation components
		glm::quat m_rotation;
		glm::mat4 m_scale;
		glm::mat4 m_translation;
	public:
		Transform();

		const glm::mat4& getModelMatrix() const { return m_modelMatrix; }

		const glm::quat& getRotation() const { return m_rotation; }
		const glm::vec3 getScale() const { return Mat4::getScale(m_scale); }
		const glm::vec3 getTranslation() const { return Mat4::getTranslation(m_translation); }

		void updateModelMatrix() { m_modelMatrix = glm::mat4_cast(m_rotation) * m_scale * m_translation; }
		void setRotation(const glm::quat &rot);
		void setScale(const glm::vec3 &scale);
		void setTranslation(const glm::vec3 &trans);
	};
}