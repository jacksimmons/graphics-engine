#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "uniform.hpp"
#include "matrix.hpp"


Camera::Camera(glm::vec3 eye, glm::vec3 centre, glm::vec3 up)
{
	m_V = glm::mat4(1.0f);
	m_R = glm::mat4(1.0f);
	m_T = glm::mat4(1.0f);

	m_eye = eye;
	m_centre = centre;
	m_up = up;

	update();
}


glm::mat4 Camera::getView()
{
	return m_V;
}


void Camera::move(glm::vec3 vec)
{
	m_T = glm::translate(m_T, vec);
}



void Camera::rotate(glm::vec3 vec)
{
	glm::vec3 yAxis = glm::normalize(m_R * glm::vec4(m_up, 1.0f));
	glm::vec3 zAxis = glm::normalize(m_R * glm::vec4(m_centre - m_eye, 1.0f));
	glm::vec3 xAxis = glm::cross(yAxis, zAxis);

	glm::quat rx = glm::angleAxis(vec.x, xAxis);
	glm::quat ry = glm::angleAxis(vec.y, yAxis);
	glm::quat rz = glm::angleAxis(vec.z, zAxis);

	glm::quat rot = ry * rz * rx;

	m_R = glm::mat4_cast(rot) * m_R;
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
	glm::vec3 t_centre = glm::vec3(m_T * Matrix::rotateAboutPoint(
		m_centre, -m_eye, m_R) * glm::vec4(m_centre, 1));
	glm::vec3 t_eye = glm::vec3(m_T * glm::vec4(m_eye, 1));
	glm::vec3 t_up = glm::vec3(m_R * glm::vec4(m_up, 1));

	m_V = glm::lookAt(t_eye, t_centre, t_up);
}