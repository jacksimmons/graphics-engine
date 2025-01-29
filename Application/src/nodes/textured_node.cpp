#include <array>
#include <string>
#include "texture.hpp"
#include "shader.hpp"
#include "node.hpp"
#include "textured_node.hpp"


namespace Tank
{
	TexturedNode::TexturedNode(const std::string &name, const std::string &vsName, const std::string &fsName) : Node(name)
	{
		m_shader = std::make_unique<Shader>(vsName, fsName);
	}


	bool TexturedNode::addTexture(const std::string &filename, GLenum mode, const std::string &uniformName)
	{
		size_t rawTexNum = m_textures.size();
		if (rawTexNum > 31) return false;

		int texNum = (int)rawTexNum;
		GLuint texID;

		if (Texture::fromFile(filename, GL_TEXTURE0 + texNum, mode, &texID))
		{
			m_textures.push_back(texID);

			m_shader->use();
			m_shader->setInt(uniformName, texNum);
			m_shader->unuse();
			return true;
		}

		return false;
	}


	bool TexturedNode::addCubeMapTexture(const std::array<std::string, 6> &filenames, GLenum mode, const std::string &uniformName)
	{
		size_t rawTexNum = m_textures.size();
		if (rawTexNum > 31) return false;

		int texNum = (int)rawTexNum;
		GLuint texID;

		if (Texture::cubeMapFromFile(filenames, GL_TEXTURE0 + texNum, mode, &texID))
		{
			m_textures.push_back(texID);

			m_shader->use();
			m_shader->setInt(uniformName, texNum);
			m_shader->unuse();
			return true;
		}

		return false;
	}
}