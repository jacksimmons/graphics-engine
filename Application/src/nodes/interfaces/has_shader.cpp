#include "has_shader.hpp"


namespace Tank
{
	IHasShader::IHasShader(const std::string &vsName,
		const std::string &fsName)
	{
		auto maybeShader = Shader::createShader(vsName, fsName);
		if (maybeShader.has_value())
			m_shader = std::move(maybeShader.value());
	}
}