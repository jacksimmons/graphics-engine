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
	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;
		GLuint m_t0, m_t1;
		std::unique_ptr<Shader> m_shader;
	public:
		Model(std::string name);
		void draw() const override;
	};
}