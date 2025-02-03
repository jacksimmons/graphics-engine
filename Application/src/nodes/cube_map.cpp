#include <string>
#include <array>
#include <memory>
#include <glad/glad.h>
#include "cube_map.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "log.hpp"
#include "texture.hpp"
#include "node.hpp"


namespace Tank
{
	CubeMap::CubeMap(const std::string &name, const std::string &vsName, const std::string &fsName, const std::array<std::string, 6> &textureNames)
		: Node(name)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), &s_vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		m_shader = std::make_unique<Shader>(vsName, fsName);
		m_shader->use();

		int texNum = Texture::getTexCount();
		auto tex = Texture::cubeMapFromFile("textures/skybox", textureNames, texNum, GL_RGB, "cubeMap");
		if (tex.has_value())
		{
			m_texture = std::make_unique<Texture>(tex.value());
			m_shader->setInt("cubeMap", texNum);
			TE_CORE_INFO(std::format("Set {} to {}, first filename {}", "cubeMap", texNum, textureNames[0]));
		}
		else
		{
			TE_CORE_ERROR(std::format("GL_TEXTURE_CUBE_MAP GL_TEXTURE{} uniform was not set.", texNum));
		}
		m_shader->unuse();
	}


	/// <summary>
	/// Must be drawn before anything else in the scene.
	/// </summary>
	void CubeMap::draw()
	{
		// Bind all owned texture objects
		m_shader->use();
		int texPos = m_texture->getTexPos();
		int texTarget = m_texture->getTexTarget();
		int texID = m_texture->getTexID();
		glActiveTexture(texPos);
		m_shader->setInt("cubeMap", texPos - GL_TEXTURE0);
		glBindTexture(texTarget, texID);

		Camera *cam = Scene::getActiveScene()->getActiveCamera();

		// Remove translation section of the transformation
		m_shader->setMat4("view", glm::mat4(glm::mat3(cam->getView())));
		m_shader->setMat4("proj", cam->getProj());

		glDepthMask(GL_FALSE);
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		Node::draw();
	}


	Skybox::Skybox(const std::string &name)
		// ! Bottom and top are switched
		: CubeMap(name, "skybox.vert", "skybox.frag", { "right.jpg", "left.jpg", "bottom.jpg", "top.jpg", "front.jpg", "back.jpg" })
	{
	}
}