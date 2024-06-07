#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "shader.hpp"


class Uniform
{
private:
	std::string m_name;
	int m_loc;
public:
	Uniform(Shader *shader, const std::string &name);
	void setInt(int value);
	void setFloat(float value);
	void setMat4(glm::mat4 value);
};