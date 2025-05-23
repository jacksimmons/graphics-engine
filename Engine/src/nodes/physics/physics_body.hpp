#pragma once
#include "transformation.hpp"
#include "nodes/node.hpp"


namespace Tank
{
	/// <summary>
	/// Base class for a Node which interacts with the physics engine.
	/// Has a mass, and exerts gravity on all bodies in the scene.
	/// </summary>
	class TANK_API PhysicsBody : public Node
	{
	private:
		static std::vector<PhysicsBody*> s_instances;
		float m_mass;
		std::vector<glm::vec3> m_velocities;

		void handleInteraction(size_t bodyIndex, float dt);
		float getGravityScalar(float distance, float otherMass) const;
	public:
		PhysicsBody(const std::string &name = "PhysicsBody", float mass = 1);
		virtual ~PhysicsBody();

		void setMass(float mass) noexcept { m_mass = mass; }
		float getMass() const noexcept { return m_mass; }

		glm::vec3 getCentre() const noexcept { return mat4::getTranslation(m_transform->getWorldModelMatrix()); }

		void update() override;
	};
}