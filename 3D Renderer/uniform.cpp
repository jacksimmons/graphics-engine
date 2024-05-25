#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "uniform.hpp"


Uniform::Uniform(Shader *shader, const std::string &name)
{
	m_loc = glGetUniformLocation(shader->getID(), name.c_str());
	m_name = name;
}


void Uniform::setInt(int value)
{
	glUniform1i(m_loc, value);
}


void Uniform::setFloat(float value)
{
	glUniform1f(m_loc, value);
}


void Uniform::setMat4(glm::mat4 value)
{
	glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(value));
}