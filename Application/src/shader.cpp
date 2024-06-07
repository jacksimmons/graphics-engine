#include <iostream>
#include <glad/glad.h>

#include "log.hpp"
#include "shader.hpp"
#include "file.hpp"


namespace Tank
{
	Shader::Shader(std::filesystem::path fpVert, std::filesystem::path fpFrag)
	{
		m_fpFrag = fpFrag;
		m_fpVert = fpVert;

		std::string vertShader;
		std::string fragShader;

		if (!File::readAllLines(std::filesystem::current_path() / fpVert, &vertShader))
		{
			TE_CORE_ERROR("Failed to read vertex shader.");
		}

		if (!File::readAllLines(std::filesystem::current_path() / fpFrag, &fragShader))
		{
			TE_CORE_ERROR("Failed to read fragment shader.");
		}

		unsigned int vshader;
		vshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vshader, 1, StringHelper(vertShader), nullptr);
		glCompileShader(vshader);

		int success;
		char infoLog[512];
		glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vshader, 512, nullptr, infoLog);
			TE_CORE_ERROR("Failed to compile vertex shader.");
			TE_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_LOG");
			TE_CORE_ERROR(infoLog);
		}

		unsigned int fshader;
		fshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fshader, 1, StringHelper(fragShader), nullptr);
		glCompileShader(fshader);

		glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vshader, 512, nullptr, infoLog);
			TE_CORE_ERROR("Failed to compile fragment shader.");
			TE_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_LOG");
			TE_CORE_ERROR(infoLog);
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, vshader);
		glAttachShader(m_id, fshader);
		glLinkProgram(m_id);

		// Cleanup
		glDeleteShader(vshader);
		glDeleteShader(fshader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}
}