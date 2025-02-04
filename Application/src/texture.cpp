#include <algorithm>
#include "texture.hpp"
#include "nodes/model.hpp"


namespace Tank
{
	int Texture::s_texCount = 0;


	Texture::Texture(GLuint texID, GLenum texTarget, const std::string &texType, const std::string &directory, const std::string &filename) :
		m_texID(texID), m_texTarget(texTarget), m_texType(texType), m_directory(directory), m_filename(filename)
	{
		s_texCount++;
	}


	Texture::~Texture()
	{
		GLuint id = getTexID();
		glDeleteTextures(1, &id);

		Model::deleteLoadedTexture(this);
	}
}