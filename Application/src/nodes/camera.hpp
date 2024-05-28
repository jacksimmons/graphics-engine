#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nodes/node.hpp>


namespace Tank
{
	class Camera : Node
	{
	private:
		// Projection matrix
		glm::mat4 m_P;
		// View matrix
		glm::mat4 m_V;
		// Camera rotation
		glm::mat4 m_R;
		// Camera translation
		glm::mat4 m_T;

		// glm::look_at params
		glm::vec3 m_eye;
		glm::vec3 m_centre;
		glm::vec3 m_up;
	public:
		Camera(glm::vec3 eye, glm::vec3 centre, glm::vec3 up, std::string name);
		glm::mat4 getView() const { return m_V; }
		glm::mat4 getProj() const { return m_P; }
		void move(glm::vec3 vec);
		void rotate(glm::vec3 vec);
		void update() override;
	};
}