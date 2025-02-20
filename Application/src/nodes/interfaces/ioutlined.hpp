#pragma once
#include <memory>
#include <functional>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <shader.hpp>
#include <nodes/node.hpp>


namespace Tank
{
	class Mesh;
	class IOutlined : public Node
	{
	private:
		std::unique_ptr<Shader> m_outlineShader;
		bool m_outlineEnabled;
	protected:
		IOutlined(const std::string &name, const glm::vec4 &outlineCol);
	public:
		virtual ~IOutlined() = default;

		void setOutlineEnabled(bool enabled) noexcept { m_outlineEnabled = enabled; }
		void predraw();
		void postdraw(const std::vector<Mesh> &meshes);
	};
}