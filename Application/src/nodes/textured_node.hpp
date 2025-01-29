#pragma once
#include <glad/glad.h>
#include "node.hpp"


namespace Tank
{
	class Shader;

	/// <summary>
	/// A node which has a shader and can have up to 32 textures.
	/// </summary>
	class TexturedNode : public Node
	{
	protected:
		TexturedNode(const std::string &name, const std::string &vsName, const std::string &fsName);
		virtual ~TexturedNode() = default;

		std::vector<GLuint> m_textures;
		std::unique_ptr<Shader> m_shader;

		bool addTexture(const std::string &filename, GLenum mode, const std::string &uniformName);
		bool addCubeMapTexture(const std::array<std::string, 6> &filenames, GLenum mode, const std::string &uniformName);
	};
}