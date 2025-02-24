#include <vector>
#include "shader.hpp"


namespace Tank
{
	Shader::Shader(unsigned id, const std::unordered_map<GLuint, fs::path> &shaders) : m_id(id), m_shaders(shaders)
	{

	}


	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}


	std::optional<std::unique_ptr<Shader>> Shader::createShader(const ShaderDict &shaders)
	{
		std::string vsString, fsString, gsString;

		unsigned progId = glCreateProgram();
		std::vector<GLuint> shadersToDelete;
		for (const auto &kvp : shaders)
		{
			std::string shaderContents;

			// Exit with error if any shader file cannot be read.
			if (!readShaderFile(kvp.second, shaderContents, "Unspecified shader")) return {};
			
			std::optional<GLuint> shader = compileShader(shaderContents, kvp.first, "Unspecified shader");
			// Exit with error if any shader file fails to compile.
			if (!shader.has_value()) return {};

			glAttachShader(progId, shader.value());
			shadersToDelete.push_back(shader.value());
		}

		glLinkProgram(progId);

		// Cleanup shaders
		for (GLuint shader : shadersToDelete)
		{
			glDeleteProgram(shader);
		}

		return std::unique_ptr<Shader>(new Shader(progId, shaders));
	}


	bool Shader::readShaderFile(const fs::path &shaderPath, std::string &shaderContents, const std::string &shaderType)
	{
		if (!File::readLines(std::filesystem::path(ROOT_DIRECTORY) / "shaders" / shaderPath, shaderContents))
		{
			std::string errMsg = "Failed to read " + shaderType + " shader: " + std::string(ROOT_DIRECTORY) +
				std::string("/shaders/") + shaderPath.string();
			TE_CORE_ERROR(errMsg);
			return false;
		}

		return true;
	}


	std::optional<unsigned> Shader::compileShader(const std::string &shaderContents, GLenum shaderType, const std::string &shaderTypeStr)
	{
		unsigned int shader;
		shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, StringHelper(shaderContents), nullptr);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			TE_CORE_ERROR("Failed to compile " + shaderTypeStr + " shader.");
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			TE_CORE_ERROR(std::string("Shader info log: ") + infoLog);

			glDeleteShader(shader);
			return {};
		}
		return shader;
	}
}