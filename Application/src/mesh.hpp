#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <shader.hpp>


namespace Tank
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};


	class Texture;
	class Mesh
	{
	private:
		unsigned int m_vao, m_vbo, m_ebo;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned> m_indices;
		std::vector<Texture> m_textures;
	public:
		Mesh(
			std::vector<Vertex> vertices,
			std::vector<unsigned> indices,
			std::vector<Texture> textures
		);
		void draw(Shader *shader) const;
	};
}