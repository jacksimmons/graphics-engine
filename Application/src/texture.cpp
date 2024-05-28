#include <glad/glad.h>
#include <iostream>

#include "texture.hpp"
#define INVALID_UINT (unsigned int)-1;


GLuint textureFromFile(const char *fn, GLenum texNum, GLenum mode)
{
	int w, h, numChannels;
	unsigned char *data = stbi_load(fn, &w, &h, &numChannels, 0); // +stbi1

	GLuint texture = INVALID_UINT;
	if (!data)
	{
		printf("imageFromFile: Failed to load image.");
		goto ret;
	}

	glGenTextures(1, &texture);
	glActiveTexture(texNum);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, mode, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	ret:
	stbi_image_free(data); // -stbi1

	return texture;
}