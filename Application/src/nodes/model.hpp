#pragma once
#include <memory>
#include <string>

#include "glm/glm.hpp"
#include "uniform.hpp"
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
		std::unique_ptr<Uniform> m_uniformP;
		std::unique_ptr<Uniform> m_uniformV;
		std::unique_ptr<Uniform> m_uniformM;
	public:
		Model(std::string name);
		void draw() const override;
	};
}