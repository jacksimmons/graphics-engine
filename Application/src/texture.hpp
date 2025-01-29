#pragma once
#include <glad/glad.h>
#include "stb_image.h"


namespace Tank
{
	namespace Texture
	{
		bool fromFile(const std::string &filename, GLenum texNum, GLenum mode, GLuint *outTex);
		bool cubeMapFromFile(const std::array<std::string, 6> &filenames, GLenum texNum, GLenum mode, GLuint *outTex);
	}
}