#include <glad/glad.h>
#include <iostream>

#include "log.hpp"
#include "texture.hpp"


namespace Tank
{
	namespace Texture
	{
		bool fromFile(std::string filename, GLenum texNum, GLenum mode, GLuint *outTex)
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
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data); // -stbi1
			return true;
		}
	}
}