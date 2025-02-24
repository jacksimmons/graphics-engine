#pragma once
#include <vector>
#include "mesh.hpp"
#include "shader_container.hpp"


namespace Tank
{
	class IMeshContainer : public IShaderContainer
	{
	protected:
		std::vector<Mesh> m_meshes;
		IMeshContainer(const std::string &name, const Shader::ShaderDict &dict)
			: IShaderContainer(name, dict)
		{
		}
	public:
		virtual ~IMeshContainer() = default;

		virtual void drawOutlineMeshes(Shader *outlineShader)
		{
			// Draw scaled up form of the object
			for (unsigned i = 0; i < m_meshes.size(); i++)
			{ // use
				m_meshes[i].draw(outlineShader);
			} // unuse
		}
	};
}