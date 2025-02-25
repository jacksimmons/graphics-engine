#include <vector>
#include <functional>
#include "shader.hpp"


namespace Tank
{
	Shader::Shader(unsigned id, const ShaderSources &sources) : m_id(id), m_sources(sources)
	{

	}


	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}


	std::optional<std::unique_ptr<Shader>> Shader::createShader(ShaderSources &sources)
	{
		std::string vsString, fsString, gsString;
		unsigned progId = glCreateProgram();
		std::vector<GLuint> shadersToDelete;

		if (!attachShader(progId, sources.vertex)) return {};
		if (!attachShader(progId, sources.fragment)) return {};
		if (sources.geometry.enabled && !attachShader(progId, sources.geometry)) return {};

		glLinkProgram(progId);

		// Cleanup shaders
		for (GLuint shader : shadersToDelete)
		{
			glDeleteProgram(shader);
		}

		return std::unique_ptr<Shader>(new Shader(progId, sources));
	}


	bool Shader::attachShader(GLuint programID, ShaderSource &source)
	{
		std::string shaderContents;

		// Exit with error if any shader file cannot be read.
		if (!readShaderFile(source.location, shaderContents, "Unspecified shader")) return false;

		std::optional<GLuint> shader = compileShader(shaderContents, source.glType, "Unspecified shader");

		// Exit with error if any shader file fails to compile.
		if (!shader.has_value()) return false;
		source.glID = shader.value();

		glAttachShader(programID, shader.value());
		return true;
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