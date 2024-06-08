#pragma once
#include <glad/glad.h>
#include "stb_image.h"


namespace Tank
{
	namespace Texture
	{
		bool fromFile(std::string filename, GLenum texNum, GLenum mode, GLuint *outTex);
	}
}