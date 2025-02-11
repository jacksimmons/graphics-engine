#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <optional>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "log.hpp"
#include "file.hpp"


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
		GLint getLoc(const std::string &name) const { return glGetUniformLocation(m_id, name.c_str()); }

		// Filepath of shader files
		std::filesystem::path m_vsPath;
		std::filesystem::path m_fsPath;

		Shader(std::filesystem::path vsPath, std::filesystem::path fsPath, unsigned int progId) : m_vsPath(vsPath), m_fsPath(fsPath), m_id(progId) {}
	public:
		~Shader();

		static std::optional<std::unique_ptr<Shader>> createShader(std::filesystem::path vsPath, std::filesystem::path fsPath)
		{
			std::string vsString;
			if (!File::readLines(std::filesystem::path(ROOT_DIRECTORY) / "shaders" / vsPath, vsString))
			{
				std::string errMsg = "Failed to read vertex shader: " + std::string(ROOT_DIRECTORY) +
					std::string("/shaders/") + vsPath.string();
				TE_CORE_ERROR(errMsg);
				return {};
			}

			std::string fsString;
			if (!File::readLines(std::filesystem::path(ROOT_DIRECTORY) / "shaders" / fsPath, fsString))
			{
				std::string errMsg = "Failed to read fragment shader: " + std::string(ROOT_DIRECTORY) +
					std::string("/shaders/") + fsPath.string();
				TE_CORE_ERROR(errMsg);
				return {};
			}

			unsigned int vshader;
			vshader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vshader, 1, StringHelper(vsString), nullptr);
			glCompileShader(vshader);

			int success;
			char infoLog[512];
			glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				TE_CORE_ERROR("Failed to compile vertex shader.");
				glGetShaderInfoLog(vshader, 512, nullptr, infoLog);
				TE_CORE_ERROR(std::string("Shader info log: ") + infoLog);

				glDeleteShader(vshader);
				return {};
			}

			unsigned int fshader;
			fshader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fshader, 1, StringHelper(fsString), nullptr);
			glCompileShader(fshader);

			glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				TE_CORE_ERROR("Failed to compile fragment shader.");
				glGetShaderInfoLog(fshader, 512, nullptr, infoLog);
				TE_CORE_ERROR(std::string("Shader info log: ") + infoLog);

				glDeleteShader(fshader);
				return {};
			}

			Shader *shader = new Shader(vsPath, fsPath, glCreateProgram());
			glAttachShader(shader->m_id, vshader);
			glAttachShader(shader->m_id, fshader);
			glLinkProgram(shader->m_id);

			// Cleanup
			glDeleteShader(vshader);
			glDeleteShader(fshader);

			return std::unique_ptr<Shader>(shader);
		}

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