#include <glad/glad.h>
#include <iostream>

#include "log.hpp"
#include "texture.hpp"


namespace Tank
{
	namespace Texture
	{
		bool fromFile(const std::string &filename, GLenum texNum, GLenum mode, GLuint *outTex)
		{
			int w, h, numChannels;
			unsigned char *data = stbi_load(filename.c_str(), &w, &h, &numChannels, 0); // +stbi1

			if (!data)
			{
				TE_CORE_ERROR("Failed to load texture from image. File:");
				TE_CORE_ERROR(filename);
				stbi_image_free(data); // -stbi1
				return false;
			}

			glGenTextures(1, outTex);
			glActiveTexture(texNum);
			glBindTexture(GL_TEXTURE_2D, *outTex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, mode, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data); // -stbi1
			glGenerateMipmap(GL_TEXTURE_2D);

			return true;
		}


		bool cubeMapFromFile(const std::array<std::string, 6> &filenames, GLenum texNum, GLenum mode, GLuint *outTex)
		{
			glGenTextures(1, outTex);
			glActiveTexture(texNum);
			glBindTexture(GL_TEXTURE_CUBE_MAP, *outTex);
			for (int i = 0; i < 6; i++)
			{
				int w, h, numChannels;
				unsigned char *data = stbi_load(filenames[i].c_str(), &w, &h, &numChannels, 0); // +stbi1
				
				if (!data)
				{
					TE_CORE_ERROR("Failed to load texture from image. File:");
					TE_CORE_ERROR(filenames[i]);
					stbi_image_free(data); // -stbi1
					return false;
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
		
			return true;
		}
	}
}