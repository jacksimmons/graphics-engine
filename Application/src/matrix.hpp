#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>


struct Mat4Decomp
{
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
};


class Matrix
{
public:
	static glm::mat4 rotateAboutPoint(glm::vec3 point, glm::vec3 origin, glm::mat4 rot);
	static glm::vec3 getTranslation(glm::mat4 mat) { return mat[3]; }
	static glm::vec3 getScale(glm::mat4 mat) { return glm::vec3(mat[0][0], mat[1][1], mat[2][2]); }
	static Mat4Decomp decompose(glm::mat4 mat)
	{
		Mat4Decomp decomp {};
		glm::decompose(mat, decomp.scale, decomp.orientation, decomp.translation, decomp.skew, decomp.perspective);
		return decomp;
	};
};