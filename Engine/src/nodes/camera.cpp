#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "transformation.hpp"
#include "static/glm_serialise.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	json Camera::serialise(Camera *cam)
	{
		json serialised = Node::serialise(cam);

		serialised["projection"] = mat4::serialise(cam->m_P);
		serialised["view"] = mat4::serialise(cam->m_V);
		serialised["rotation"] = mat4::serialise(cam->m_R);
		serialised["translation"] = mat4::serialise(cam->m_T);

		serialised["eye"] = vec3::serialise(cam->m_eye);
		serialised["centre"] = vec3::serialise(cam->m_centre);
		serialised["up"] = vec3::serialise(cam->m_up);

		serialised["panSpd"] = cam->m_panSpeed;
		serialised["rotSpd"] = cam->m_rotSpeed;

		return serialised;
	}


	void Camera::deserialise(const json &serialised, Camera **targetPtr)
	{
		if (!(*targetPtr)) *targetPtr = new Camera();

		Camera *camera = *targetPtr;
		camera->m_P = mat4::deserialise(serialised["projection"]);
		camera->m_V = mat4::deserialise(serialised["view"]);
		camera->m_R = mat4::deserialise(serialised["rotation"]);
		camera->m_T = mat4::deserialise(serialised["translation"]);

		camera->m_eye = vec3::deserialise(serialised["eye"]);
		camera->m_centre = vec3::deserialise(serialised["centre"]);
		camera->m_up = vec3::deserialise(serialised["up"]);

		camera->m_panSpeed = serialised["panSpd"];
		camera->m_rotSpeed = serialised["rotSpd"];

		Node *target = *targetPtr;
		Node::deserialise(serialised, &target);
	}


	Camera::Camera(const std::string &name, glm::vec3 eye, glm::vec3 centre, glm::vec3 up) : Node(name)
	{
		m_type = "Camera";

		// Create a perspective projection for this camera.
		m_P = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		m_V = glm::mat4(1.0f);
		m_R = glm::mat4(1.0f);
		m_T = glm::mat4(1.0f);

		m_eye = eye;
		m_centre = centre;
		m_up = up;

		m_panSpeed = 1;
		m_rotSpeed = 1;
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

		glm::quat rot = quat::fromAngleAxis(glm::vec3(vec.y, vec.x, vec.z), xAxis, yAxis, zAxis);

		m_R = glm::mat4_cast(rot) * m_R;
	}

	glm::vec3 Camera::getTransformedCentre() const
	{
		return glm::vec3(m_T * mat4::rotateAboutPoint(m_centre, -m_eye, m_R) * glm::vec4(m_centre, 1));
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