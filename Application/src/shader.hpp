#pragma once
#include <string>
#include <glad/glad.h>


class ShaderCompileError : std::exception
{
private:
	std::string m_message;
public:
	ShaderCompileError(std::string pipelineStage);
	const char *what() const noexcept override
	{
		return m_message.c_str();
	}
};


/// <summary>
/// A class for elegantly passing const char ** arguments to
/// OpenGL functions.
/// </summary>
class StringHelper
{
private:
	const char *m_str;
public:
	StringHelper(const std::string &s) : m_str(s.c_str()) {}
	operator const char **() {
		return &m_str;
	}
};


/// <summary>
/// A Shader represents a shader program in OpenGL.
/// Consists of all parts of the rendering pipeline.
/// </summary>
class Shader
{
private:
	GLuint m_id;
public:
	Shader(std::string fp_vert, std::string fp_frag);
	~Shader();

	void use() const { glUseProgram(m_id); }
	GLuint getID() const { return m_id; };
};