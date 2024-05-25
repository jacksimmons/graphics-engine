#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


class Camera
{
private:
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
	Camera(glm::vec3 eye, glm::vec3 centre, glm::vec3 up);
	glm::mat4 getView();
	void move(glm::vec3 vec);
	void rotate(glm::vec3 vec);
	void update();
};