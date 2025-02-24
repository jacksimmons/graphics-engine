#pragma once
#include <memory>
#include <glad/glad.h>
#include <nodes/node.hpp>
#include "shader.hpp"


namespace Tank
{
	class Node; class Texture;
	class CubeMap : public Node
	{
		// Serialisation
	public:
		static json serialise(CubeMap *skybox);
		static void deserialise(const json &serialised, CubeMap **targetPtr);

	protected:
		GLuint m_vao;
		GLuint m_vbo;
		std::unique_ptr<Shader> m_shader;
		std::shared_ptr<Texture> m_texture;
		std::array<std::string, 6> m_texturePaths;
	public:
		CubeMap(const std::string &name = "CubeMap (Skybox)",
			const Shader::ShaderDict &dict = { {GL_VERTEX_SHADER, "skybox.vert"}, {GL_FRAGMENT_SHADER, "skybox.frag"} },
			const std::array<std::string, 6> &textureNames = { "right.jpg", "left.jpg", "bottom.jpg", "top.jpg", "front.jpg", "back.jpg" }
		);

		virtual void draw() override;

	private:
		constexpr static float s_vertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
	};
}