#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "transformation.hpp"


namespace Tank
{
	class Node;
	class Transform
	{
	private:
		// Entire transformation
		glm::mat4 m_modelMatrix;
		Node *m_owner;

		// Individual transformation components
		glm::quat m_rotation;
		glm::vec3 m_scale;
		glm::vec3 m_translation;

		glm::mat4 getModelMatrix() const;
	public:
		Transform(Node *owner = nullptr);
		virtual ~Transform() {};

		/// <summary>
		/// Recursively pre-multiplies the model matrix by parent's getWorldMatrix result.
		/// </summary>
		glm::mat4 getWorldMatrix() const;

		const glm::quat& getRotation() const { return m_rotation; }
		const glm::vec3& getScale() const { return m_scale; }
		const glm::vec3& getTranslation() const { return m_translation; }

		void setRotation(const glm::quat &rot) { m_rotation = rot; }
		void setScale(const glm::vec3 &scale) { m_scale = scale; }
		void setTranslation(const glm::vec3 &trans) { m_translation = trans; }
		void setOwner(Node *owner) { m_owner = owner; }
	};
}