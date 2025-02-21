#include <algorithm>
#include "has_shader.hpp"


namespace Tank
{
	std::vector<std::weak_ptr<Texture>> IHasShader::s_loadedTextures;


	IHasShader::IHasShader(const std::string &vsName,
		const std::string &fsName)
	{
		auto maybeShader = Shader::createShader(vsName, fsName);
		if (maybeShader.has_value())
			m_shader = std::move(maybeShader.value());
	}


	void IHasShader::touchLoadedTextures()
	{
		std::vector<size_t> toRemoveIndices;
		for (size_t i = 0; i < s_loadedTextures.size(); i++)
		{
			if (s_loadedTextures[i].expired()) toRemoveIndices.push_back(i);
		}

		for (const size_t toRemoveIndex : toRemoveIndices)
		{
			s_loadedTextures.erase(s_loadedTextures.begin() + toRemoveIndex);
		}
	}


	std::vector<std::shared_ptr<Texture>> IHasShader::getLoadedTextures()
	{
		touchLoadedTextures();

		std::vector<std::shared_ptr<Texture>> textures;
		textures.resize(s_loadedTextures.size());
		std::transform(
			s_loadedTextures.begin(),
			s_loadedTextures.end(),
			textures.begin(),
			[](std::weak_ptr<Texture> tex) { return tex.lock(); }
		);
		return textures;
	}

	
	void IHasShader::addLoadedTexture(std::weak_ptr<Texture> texture)
	{
		s_loadedTextures.push_back(texture);
	}
}