#pragma once
#include <memory>
#include <string>

#include "glm/glm.hpp"
#include "shader.hpp"
#include "nodes/node.hpp"


namespace Tank
{
	class Model : public Node
	{
		friend class Inspector;
	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;
	protected:
		std::vector<GLuint> m_textures;
		std::unique_ptr<Shader> m_shader;
		bool addTexture(std::string filename, GLenum mode, std::string uniformName);
	public:
		Model(const std::string &name,
			const std::string &vsName,
			const std::string &fsName,
			float *vertices,
			size_t verticesSize);

		Shader *getShader() const noexcept { return m_shader.get(); };

		void draw() override;
	};
}