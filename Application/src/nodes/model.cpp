#include <iostream>
#include <filesystem>
#include <array>
#include <memory>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "log.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "scene.hpp"
#include "transform.hpp"
#include "nodes/model.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	Model::Model(const std::string &name, const std::string &vsName, const std::string &fsName, float *vertices, size_t verticesSize)
		: TexturedNode(name, vsName, fsName)
	{
		// ===== INIT VBO/VAO =====
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//// Colour
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		// Texture coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Textures
		float borderColour[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}


	void Model::draw()
	{
		for (int i = 0; i < m_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		}

		m_shader->use();

		glm::mat4 model = getTransform()->getWorldMatrix();
		m_shader->setMat4("model", model);
		m_shader->setMat3("normalMatrix", glm::mat3(glm::inverseTranspose(model)));

		auto cam = Scene::getActiveScene()->getActiveCamera();
		m_shader->setMat4("view", cam->getView());
		m_shader->setMat4("proj", cam->getProj());
		m_shader->setVec3("viewPos", cam->getTransformedEye());
		m_shader->setVec3("objectCol", { 1.0f, 1.0f, 1.0f });

		m_shader->setFloat("material.shininess", 32.0f);

		std::vector<Light *> lights = Scene::getActiveScene()->getActiveLights();
		for (int i = 0; i < lights.size(); i++)
		{
			lights[i]->updateShader(m_shader.get());
		}

		m_shader->unuse();

		TexturedNode::draw();
	}


	void Model::update()
	{
		for (Light *light : Scene::getActiveScene()->getActiveLights())
		{
			light->updateShader(m_shader.get());
		}

		Node::update();
	}
}