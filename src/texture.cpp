#include "texture.hpp"


namespace Tank
{
	GLuint Texture::s_texCount = 0;


	Texture::Texture(GLuint texID, GLuint texPos) : m_texID(texID), m_texPos(texPos)
	{ 
		s_texCount++;
	};


	Texture::~Texture()
	{
		// !Need to free up GL_TEXTURE0 + texPos
		// !But this creates a gap that will never be filled by the current system.
	}
}