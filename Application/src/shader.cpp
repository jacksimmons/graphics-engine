#include "shader.hpp"


namespace Tank
{
	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}
}