#pragma once
#include <glad/glad.h>
#include "stb_image.h"


typedef GLuint Texture;


Texture textureFromFile(const char *fn, GLenum texNum, GLenum mode);