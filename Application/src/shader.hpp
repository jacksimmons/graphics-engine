#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


namespace Tank
{
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
		std::filesystem::path m_vsPath;
		std::filesystem::path m_fsPath;
	public:
		Shader(std::filesystem::path vsPath, std::filesystem::path fsPath);
		~Shader();

		void use() const { glUseProgram(m_id); }
		void unuse() const { glUseProgram(0); }
		GLuint getID() const noexcept { return m_id; };
		std::filesystem::path getVertPath() const noexcept { return m_vsPath; }
		std::filesystem::path getFragPath() const noexcept { return m_fsPath; }
		void setInt(const std::string &name, int value) const
		{
			glUniform1i(getLoc(name.c_str()), value);
		}
		void setFloat(const std::string &name, float value) const
		{
			glUniform1f(getLoc(name.c_str()), value);
		}
		void setVec3(const std::string &name, const glm::vec3 &value) const
		{
			glUniform3fv(getLoc(name.c_str()), 1, &value[0]);
		}
		void setVec4(const std::string &name, const glm::vec4 &value) const
		{
			glUniform4fv(getLoc(name.c_str()), 1, &value[0]);
		}
		void setMat3(const std::string &name, const glm::mat3 &value) const
		{
			glUniformMatrix3fv(getLoc(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
		}
		void setMat4(const std::string &name, const glm::mat4 &value) const
		{
			glUniformMatrix4fv(getLoc(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
		}
	};
}