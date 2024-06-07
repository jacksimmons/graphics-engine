#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "uniform.hpp"


Uniform::Uniform(Shader *shader, const std::string &name)
{
	m_loc = glGetUniformLocation(shader->getID(), name.c_str());
	m_name = name;
}