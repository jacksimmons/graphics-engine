#pragma once
#include <memory>
#include <string>

#include "glm/glm.hpp"
#include "shader.hpp"
#include "nodes/textured_node.hpp"


namespace Tank
{
	class Light;
	class Model : public TexturedNode
	{
		friend class Inspector;
	protected:
		unsigned int m_vao;
		unsigned int m_vbo;
	public:
		Model(const std::string &name,
			const std::string &vsName,
			const std::string &fsName,
			float *vertices,
			size_t verticesSize);

		virtual void draw() override;
		virtual void update() override;
	};
}