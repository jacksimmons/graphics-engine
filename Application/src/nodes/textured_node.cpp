#include <array>
#include <string>
#include <memory>
#include "texture.hpp"
#include "shader.hpp"
#include "node.hpp"
#include "textured_node.hpp"
#include "texture.hpp"


namespace Tank
{
	TexturedNode::TexturedNode(const std::string &name, const std::string &vsName, const std::string &fsName) : Node(name)
	{
		m_shader = std::make_unique<Shader>(vsName, fsName);
	}


	bool TexturedNode::addTexture(const std::string &filename, GLenum mode, const std::string &uniformName)
	{
		int texNum = Texture::getTexCount();
		auto tex = Texture::fromFile(filename, GL_TEXTURE0 + texNum, mode);
		if (tex.has_value())
		{
			m_textures.push_back(std::move(tex.value()));

			m_shader->use();
			m_shader->setInt(uniformName, texNum);
			m_shader->unuse();
			return true;
		}

		return false;
	}


	bool TexturedNode::addCubeMapTexture(const std::array<std::string, 6> &filenames, GLenum mode, const std::string &uniformName)
	{
		int texNum = Texture::getTexCount();
		auto tex = Texture::cubeMapFromFile(filenames, GL_TEXTURE0 + texNum, mode);
		if (tex.has_value())
		{
			m_textures.push_back(std::move(tex.value()));

			m_shader->use();
			m_shader->setInt(uniformName, texNum);
			m_shader->unuse();
			return true;
		}

		return false;
	}
}