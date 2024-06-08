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
	public:
		Model(std::string name, std::string vsName, std::string fsName);
		bool addTexture(std::string filename, GLenum mode, std::string uniformName);
		void draw() const override;
	};
}