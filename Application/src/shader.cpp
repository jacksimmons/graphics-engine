#include <iostream>

#include <glad/glad.h>

#include "shader.hpp"
#include "file.hpp"


ShaderCompileError::ShaderCompileError(std::string pipelineStage)
{
	m_message = "Failed to compile " + pipelineStage + " shader.";
}


/// <summary>
/// Generates a shader program from shader files.
/// </summary>
/// <returns>The shader program, as a uint.</returns>
Shader::Shader(std::filesystem::path fpVert, std::filesystem::path fpFrag)
{
	m_fpFrag = fpFrag;
	m_fpVert = fpVert;

	std::string vertShader = File::readAllLines(std::filesystem::current_path() / fpVert);
	std::string fragShader = File::readAllLines(std::filesystem::current_path() / fpFrag);
	
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
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_LOG\n" << infoLog << std::endl;
		throw ShaderCompileError("Vertex");
	}

	unsigned int fshader;
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, StringHelper(fragShader), nullptr);
	glCompileShader(fshader);

	glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vshader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_LOG\n" << infoLog << std::endl;
		throw ShaderCompileError("Fragment");
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