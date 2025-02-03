#include <iostream>
#include <filesystem>
#include <array>
#include <memory>
#include <format>
#include <algorithm>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

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
	std::vector<Texture> Model::s_texturesLoaded;


	Model::Model(const std::string &name, const std::string &vsName, const std::string &fsName, const std::string &modelPath)
		: Node(name)
	{
		m_shader = std::make_unique<Shader>(vsName, fsName);

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			TE_CORE_ERROR(std::format("ASSIMP: {}", importer.GetErrorString()));
			return;
		}

		m_directory = modelPath.substr(0, modelPath.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}


	void Model::processNode(aiNode *node, const aiScene *scene)
	{
		// Process all of node's meshes
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(processMesh(mesh, scene));
		}

		// Recurse on children's meshes
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}


	Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// Vertices
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vert;
			vert.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vert.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			// If mesh has tex coords
			if (mesh->mTextureCoords[0])
			{
				vert.texCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			else
			{
				vert.texCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vert);
		}

		// Indices
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Materials
		if (mesh->mMaterialIndex >= 0)
		{
			m_shader->use();
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture> diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
			textures.insert(textures.end(), diffuse.begin(), diffuse.end());

			std::vector<Texture> specular = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
			textures.insert(textures.end(), specular.begin(), specular.end());
			m_shader->unuse();
		}

		return Mesh(vertices, indices, textures);
	}


	std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (size_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skipLoading = false;

			// See if texture with same path has already been loaded. If it has, copy existing version.
			for (size_t j = 0; j < s_texturesLoaded.size(); j++)
			{
				if (std::strcmp(s_texturesLoaded[j].getFilename().data(), str.C_Str()) == 0)
				{
					textures.push_back(s_texturesLoaded[j]);
					skipLoading = true;
					break;
				}
			}

			if (!skipLoading)
			{
				auto tex = Texture::fromFile(m_directory, str.C_Str(), Texture::getTexCount(), typeName);

				if (tex.has_value())
				{
					Texture val = tex.value();
					textures.push_back(val);
					s_texturesLoaded.push_back(val);
				}
				else
				{
					TE_CORE_ERROR(std::format("Unable to load texture {}/{}", m_directory, str.C_Str()));
				}
			}
		}

		return textures;
	}


	void Model::draw()
	{
		m_shader->use();
		m_shader->setVec3("tex_scale", glm::vec3{ 1, 1, 1 });
		m_shader->setFloat("material.Ns", 32.0f);

		auto cam = Scene::getActiveScene()->getActiveCamera();
		auto P = cam->getProj();
		auto V = cam->getView();
		auto M = getTransform()->getWorldModelMatrix();
		auto VM = V * M;
		m_shader->setMat4("PVM", P * VM);
		m_shader->setMat4("VM", VM);
		m_shader->setMat4("V", V);
		m_shader->setMat4("VM_it", glm::inverseTranspose(VM));

		for (Light *light : Scene::getActiveScene()->getActiveLights())
		{
			light->updateShader(m_shader.get());
		}
		m_shader->setInt("num_dir_lights", DirLight::getCount());
		m_shader->setInt("num_point_lights", PointLight::getCount());

		for (size_t i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i].draw(m_shader.get());
		}

		m_shader->unuse();

		Node::draw();
	}


	void Model::update()
	{
		Node::update();
	}
}