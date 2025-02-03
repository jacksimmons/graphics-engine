#include "texture.hpp"


namespace Tank
{
	GLuint Texture::s_texCount = 0;


	Texture::Texture(GLuint texID, GLuint texPos, GLenum texTarget, const std::string &texType,
		const std::string &directory, const std::string &filename) :
		m_texID(texID), m_texPos(texPos), m_texTarget(texTarget), m_texType(texType), m_directory(directory), m_filename(filename)
	{
		s_texCount++;
	};


	Texture::~Texture()
	{
		// !Need to free up GL_TEXTURE0 + texPos
		// !But this creates a gap that will never be filled by the current system.
	}
}