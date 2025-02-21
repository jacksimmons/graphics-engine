#include <algorithm>
#include "texture.hpp"
#include "nodes/model.hpp"


namespace Tank
{
	unsigned Texture::s_numTextures = 0;


	Texture::Texture(GLuint texID, GLenum texTarget, const std::string &texType, const std::string &directory, const std::string &filename) :
		m_texID(texID), m_texTarget(texTarget), m_texType(texType), m_directory(directory), m_filename(filename)
	{
		s_numTextures++;
	}


	Texture::~Texture()
	{
		GLuint id = getTexID();
		glDeleteTextures(1, &id);

		s_numTextures--;
	}
}