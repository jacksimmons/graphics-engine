#pragma once
#include <memory>
#include <vector>
#include <string>
#include <shader.hpp>


namespace Tank
{
	class Texture;
	class IHasShader
	{
	protected:
		std::unique_ptr<Shader> m_shader;

		IHasShader(const std::string &vsName,
			const std::string &fsName);
	public:
		virtual ~IHasShader() = default;

		void setShader(std::unique_ptr<Shader> shader) { m_shader = std::move(shader); };
		Shader *getShader() const { return m_shader.get(); }
	};
}