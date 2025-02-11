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
#include "transform.hpp"
#include "nodes/scene.hpp"
#include "nodes/model.hpp"
#include "nodes/camera.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	json Model::serialise(Model *model)
	{
		json serialised = Node::serialise(model);
		serialised["vsPath"] = model->m_shader->getVertPath().string();
		serialised["fsPath"] = model->m_shader->getFragPath().string();
		serialised["modelPath"] = model->m_modelDirectory + "/" + model->m_modelFile;
		return serialised;
	}


	void Model::deserialise(const json &serialised, Model **targetPtr)
	{
		if (!(*targetPtr)) *targetPtr = new Model(serialised["name"], serialised["vsPath"], serialised["fsPath"], serialised["modelPath"]);
		Node *target = *targetPtr;
		Node::deserialise(serialised, &target);
	}


	// This will be the last object alive to own a reference to any Texture object.
	std::vector<std::shared_ptr<Texture>> Model::s_loadedTextures;


	Model::Model(const std::string &name, const std::string &vsName, const std::string &fsName, const std::string &modelPath)
		: Node(name)
	{
		m_type = "Model";
		std::string fullModelPath = (std::string(ROOT_DIRECTORY) + "/Models/" + modelPath);
		size_t indexOfLastSlash = fullModelPath.find_last_of("/");
		m_modelDirectory = fullModelPath.substr(0, indexOfLastSlash);
		m_modelFile = fullModelPath.substr(indexOfLastSlash + 1, (fullModelPath.length() - indexOfLastSlash) + 1);

		auto maybeShader = Shader::createShader(vsName, fsName);
		if (maybeShader.has_value())
			m_shader = std::move(maybeShader.value());

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(fullModelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			TE_CORE_ERROR(std::format("ASSIMP: {}", importer.GetErrorString()));
			return;
		}

		TE_CORE_INFO(m_modelFile);
		processNode(scene->mRootNode, scene);
	}


	void Model::processNode(aiNode *node, const aiScene *scene)
	{
		// Process all of node's meshes
		for (unsigned i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(processMesh(mesh, scene));
		}

		// Recurse on children's meshes
		for (unsigned i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}


	Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		// Vertices
		for (unsigned i = 0; i < mesh->mNumVertices; i++)
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
		for (unsigned i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Materials
		if (mesh->mMaterialIndex >= 0)
		{
			m_shader->use();
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
			textures.insert(textures.end(), diffuse.begin(), diffuse.end());

			auto specular = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
			textures.insert(textures.end(), specular.begin(), specular.end());
			m_shader->unuse();
		}

		return Mesh(vertices, indices, textures);
	}


	std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		std::vector<std::shared_ptr<Texture>> textures;
		for (unsigned i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skipLoading = false;

			// See if texture with same path has already been loaded. If it has, copy existing version.
			for (unsigned j = 0; j < s_loadedTextures.size(); j++)
			{
				if (std::strcmp(s_loadedTextures[j]->getFilename().data(), str.C_Str()) == 0)
				{
					textures.push_back(s_loadedTextures[j]);
					skipLoading = true;
					break;
				}
			}

			if (!skipLoading)
			{
				auto tex = Texture::fromFile(m_modelDirectory, str.C_Str(), typeName);

				if (tex.has_value())
				{
					std::shared_ptr<Texture> val = tex.value();
					textures.push_back(val);
					s_loadedTextures.push_back(val);
				}
				else
				{
					TE_CORE_ERROR(std::format("Unable to load texture {}/{}", m_modelDirectory, str.C_Str()));
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

		auto scene = Scene::getActiveScene();
		auto activeLights = scene->getLights();
		for (Light *light : activeLights)
		{
			light->updateShader(m_shader.get());
		}
		m_shader->setInt("num_dir_lights", scene->getNumDirLights());
		m_shader->setInt("num_point_lights", scene->getNumPointLights());

		for (unsigned i = 0; i < m_meshes.size(); i++)
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