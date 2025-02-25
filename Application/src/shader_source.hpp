#pragma once
#include <glad/glad.h>
#include <filesystem>
namespace fs = std::filesystem;


namespace Tank
{
	struct ShaderSource
	{
		GLenum glType;
		GLuint glID;
		fs::path location;
		bool enabled;


		ShaderSource();
		ShaderSource(const ShaderSource &original);
		bool operator==(const ShaderSource &other);
	};


	struct ShaderSources
	{
		ShaderSource vertex;
		ShaderSource fragment;
		ShaderSource geometry;


		ShaderSources();
		ShaderSources(const ShaderSources &original);
		bool operator==(const ShaderSources &other);
	};


	/// <summary>
	/// A class for elegantly passing const char** arguments to
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
}