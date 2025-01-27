#include <glm/gtc/matrix_transform.hpp>

#include "nodes/camera.hpp"
#include "transformation.hpp"


namespace Tank
{
	Camera::Camera(const std::string &name, glm::vec3 eye, glm::vec3 centre, glm::vec3 up) : Node(name)
	{
		m_P = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		m_V = glm::mat4(1.0f);
		m_R = glm::mat4(1.0f);
		m_T = glm::mat4(1.0f);

		m_eye = eye;
		m_centre = centre;
		m_up = up;

		m_panSpeed = 0.01f;
		m_rotSpeed = 0.01f;

		update();
	}

	void Camera::setPosition(glm::vec3 pos)
	{
		m_T = glm::translate(glm::mat4(1.0f), pos);
	}

	void Camera::translate(glm::vec3 vec)
	{
		m_T = glm::translate(m_T, glm::vec3(m_R * glm::vec4(vec, 1.0f)));
	}

	void Camera::setRotation(glm::quat rot)
	{
		m_R = glm::mat4_cast(rot);
	}

	void Camera::rotate(glm::vec3 vec)
	{
		glm::vec3 yAxis = glm::normalize(m_R * glm::vec4(m_up, 1.0f));
		glm::vec3 zAxis = glm::normalize(m_R * glm::vec4(m_centre - m_eye, 1.0f));
		glm::vec3 xAxis = glm::cross(yAxis, zAxis);

		glm::quat rot = Quat::fromAngleAxis(glm::vec3(vec.y, vec.x, vec.z), xAxis, yAxis, zAxis);

		m_R = glm::mat4_cast(rot) * m_R;
	}

	glm::vec3 Camera::getTransformedCentre() const
	{
		return glm::vec3(m_T * Mat4::rotateAboutPoint(m_centre, -m_eye, m_R) * glm::vec4(m_centre, 1));
	}

	glm::vec3 Camera::getTransformedEye() const
	{
		return glm::vec3(m_T * glm::vec4(m_eye, 1));
	}

	glm::vec3 Camera::getTransformedUp() const
	{
		return glm::vec3(m_R * glm::vec4(m_up, 1));
	}

	/// <summary>
	/// 1. Rotate center around the eye("Universe rotates around camera")
	/// (Translate center by - eye, then rotate it around origin with R)
	/// (Then translate the new center by eye)
	/// 2. Translate the eye with the displacement matrix(Camera movement)
	/// 3. Rotate the up vector around origin with R.
	/// Combining these gives the view matrix.
	/// </summary>
	void Camera::update()
	{
		// Transformed vector
		glm::vec3 t_centre = getTransformedCentre();
		glm::vec3 t_eye = getTransformedEye();
		glm::vec3 t_up = getTransformedUp();
		m_V = glm::lookAt(t_eye, t_centre, t_up);
	}
}