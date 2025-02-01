#include "sprite.hpp"
#include "log.hpp"


namespace Tank
{
	Sprite::Sprite(
		const std::string &name,
		const std::string &diffuse,
		const std::string &specular,
		const std::string &vsName,
		const std::string &fsName
	) : Model(name, vsName, fsName, (float *)&s_vertices, sizeof(s_vertices))
	{
		if (!addTexture(diffuse, GL_RGBA, "material.diffuse"))
			TE_CORE_ERROR("Failed to load diffuse texture.");
		if (!addTexture(specular, GL_RGBA, "material.specular"))
			TE_CORE_ERROR("Failed to load specular texture.");
	}


	void Sprite::draw()
	{
		Model::draw();

		m_shader->use();
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		m_shader->unuse();
	}
}