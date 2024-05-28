#include <glm/gtc/matrix_transform.hpp>

#include "matrix.hpp"


glm::mat4 Matrix::rotateAboutPoint(glm::vec3 point, glm::vec3 origin, glm::mat4 rot)
{
	glm::mat4 T_PO = glm::translate(glm::mat4(1.0f), origin - point);
	glm::mat4 T_OP = glm::translate(glm::mat4(1.0f), point - origin);
	return T_OP * rot * T_PO;
}
