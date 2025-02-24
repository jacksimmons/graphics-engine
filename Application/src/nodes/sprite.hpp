#pragma once
#include <string>
#include "interfaces/mesh_container.hpp"
namespace fs = std::filesystem;


namespace Tank
{
	class Sprite : public IMeshContainer
	{
	public:
		Sprite(const std::string &name,
			const Shader::ShaderDict &dict,
			const fs::path &spritePath
		);
		virtual ~Sprite() = default;

		virtual void draw() override;
	};
}