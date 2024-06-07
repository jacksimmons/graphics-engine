#pragma once
#include <string>
#include <filesystem>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


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
	GLuint getLoc(const std::string &name) const { return glGetUniformLocation(m_id, name.c_str()); }

	// Filepath of shader files
	std::filesystem::path m_fpVert;
	std::filesystem::path m_fpFrag;
public:
	Shader(std::filesystem::path fpVert, std::filesystem::path fpFrag);
	~Shader();

	void use() const { glUseProgram(m_id); }
	void unuse() const { glUseProgram(0); }
	GLuint getID() const noexcept { return m_id; };
	void setInt(const std::string &name, int value) const { glUniform1i(getLoc(name.c_str()), value); }
	void setFloat(const std::string &name, float value) const { glUniform1f(getLoc(name.c_str()), value); }
	void setMat4(const std::string &name, glm::mat4 value) const { glUniformMatrix4fv(getLoc(name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); }
	std::filesystem::path getVertPath() const noexcept { return m_fpVert; }
	std::filesystem::path getFragPath() const noexcept { return m_fpFrag; }
};