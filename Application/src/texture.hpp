#pragma once
#include <glad/glad.h>
#include "stb_image.h"
#include "log.hpp"


namespace Tank
{
	class Texture
	{
	private:
		static GLuint s_texCount;

		GLuint m_texID; // OpenGL's internal value for this texture
		GLuint m_texPos; // GL_TEXTURE0 + m_texOffset

		// No reason to expose this constructor. Textures must be built using the factories below
		Texture(GLuint texID, GLuint texPos);
	public:
		~Texture();
		GLuint getTexID() const noexcept { return m_texID; };
		GLuint getTexPos() const noexcept { return m_texPos; };
		static GLuint getTexCount() { return s_texCount; };


		static std::optional<std::unique_ptr<Texture>> fromFile(const std::string &filename, GLuint texPos, GLenum mode)
		{
			GLint maxTextureUnits;
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
			if (s_texCount >= maxTextureUnits) return {};

			int w, h, numChannels;
			unsigned char *data = stbi_load(filename.c_str(), &w, &h, &numChannels, 0); // +stbi1

			TE_CORE_INFO("HI");
			if (!data)
			{
				TE_CORE_ERROR("Failed to load texture from image. File:");
				TE_CORE_ERROR(filename);
				stbi_image_free(data); // -stbi1
				return {};
			}

			GLuint texID;
			glGenTextures(1, &texID);
			glActiveTexture(texPos);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, mode, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data); // -stbi1
			glGenerateMipmap(GL_TEXTURE_2D);

			// Textures
			float borderColour[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			return std::unique_ptr<Texture>(new Texture(texID, texPos));
		}


		static std::optional<std::unique_ptr<Texture>> cubeMapFromFile(const std::array<std::string, 6> &filenames, GLuint texPos, GLenum mode)
		{
			GLint maxTextureUnits;
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
			if (s_texCount >= maxTextureUnits) return {};

			GLuint texID;
			glGenTextures(1, &texID);
			glActiveTexture(texPos);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
			for (int i = 0; i < 6; i++)
			{
				int w, h, numChannels;
				unsigned char *data = stbi_load(filenames[i].c_str(), &w, &h, &numChannels, 0); // +stbi1

				if (!data)
				{
					TE_CORE_ERROR("Failed to load texture from image. File:");
					TE_CORE_ERROR(filenames[i]);
					stbi_image_free(data); // -stbi1
					return {};
				}

				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, w, h, 0, mode, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data); // -stbi1
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			return std::unique_ptr<Texture>(new Texture(texID, texPos));
		}
	};
}