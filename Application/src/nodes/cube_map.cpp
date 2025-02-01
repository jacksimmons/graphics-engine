#include <string>
#include <array>
#include <glad/glad.h>
#include "cube_map.hpp"
#include "textured_node.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "log.hpp"
#include "texture.hpp"


namespace Tank
{
	CubeMap::CubeMap(const std::string &name, const std::string &vsName, const std::string &fsName, const std::array<std::string, 6> &textureNames)
		: TexturedNode(name, vsName, fsName)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), &s_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		addCubeMapTexture(textureNames, GL_RGB, "cubeMap");
	}


	/// <summary>
	/// Must be drawn before anything else in the scene.
	/// </summary>
	void CubeMap::draw()
	{
		Camera *cam = Scene::getActiveScene()->getActiveCamera();

		m_shader->use();
		// Remove translation section of the transformation
		m_shader->setMat4("view", glm::mat4(glm::mat3(cam->getView())));
		m_shader->setMat4("proj", cam->getProj());

		glDepthMask(GL_FALSE);
		glBindVertexArray(m_vao);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[0]->getTexID());
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		TexturedNode::draw();
	}


	Skybox::Skybox(const std::string &name)
		// ! Bottom and top are switched
		: CubeMap(name, "skybox.vert", "skybox.frag", { "textures/skybox/right.jpg", "textures/skybox/left.jpg", "textures/skybox/bottom.jpg", "textures/skybox/top.jpg", "textures/skybox/front.jpg", "textures/skybox/back.jpg" })
	{
	}
}