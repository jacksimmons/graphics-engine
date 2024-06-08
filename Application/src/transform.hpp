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
		glm::vec3 m_scale;
		glm::vec3 m_translation;
	public:
		Transform();

		const glm::mat4& getModelMatrix() const { return m_modelMatrix; }

		const glm::quat& getRotation() const { return m_rotation; }
		const glm::vec3& getScale() const { return m_scale; }
		const glm::vec3& getTranslation() const { return m_translation; }

		void updateModelMatrix();
		void setRotation(const glm::quat &rot) { m_rotation = rot; updateModelMatrix(); }
		void setScale(const glm::vec3 &scale) { m_scale = scale; updateModelMatrix(); }
		void setTranslation(const glm::vec3 &trans) { m_translation = trans; updateModelMatrix(); }
	};
}