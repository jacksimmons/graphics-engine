#pragma once
#include <format>
#include <string>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include "stb_image.h"
#include "log.hpp"


namespace Tank
{
	class Texture
	{
	private:
		static unsigned s_numTextures;
		GLuint m_texID; // OpenGL's internal value for this texture
		GLenum m_texTarget; // GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP
		std::string m_texType; // material.{diffuse | specular}
		std::string m_directory;
		std::string m_filename;

		// No reason to expose this constructor. Textures must be built using the factories below
		Texture(GLuint texID, GLenum texTarget, const std::string &uniformName,
			const std::string &directory, const std::string &filename);
	public:
		static unsigned getTexCount() { return s_numTextures; };

		~Texture();
		GLuint getTexID() const noexcept { return m_texID; };
		GLenum getTexTarget() const noexcept { return m_texTarget; }
		const std::string &getTexType() const { return m_texType; }
		const std::string &getDir() const { return m_directory; }
		const std::string &getFilename() const { return m_filename; }


		static std::optional<std::shared_ptr<Texture>> fromFile(const std::string &directory, const std::string &filename, const std::string &texType);


		static std::optional<std::shared_ptr<Texture>> cubeMapFromFile(const std::string &directory, const std::array<std::string, 6> &filenames,
			const std::string &texType);
	};
}