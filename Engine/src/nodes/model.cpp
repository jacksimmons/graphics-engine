#include <format>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"

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
	json Model::serialise()
	{
		json serialised = Node::serialise();
		serialised["modelPath"] = getModelPath();
		serialised["shader"] = Shader::serialise(*(m_shader));
		return serialised;
	}


	void Model::deserialise(const json &serialised, Model **targetPtr)
	{
		ShaderSources sources;
		sources.vertex.location = std::string{ serialised["shader"]["vert"] };
		sources.fragment.location = std::string{ serialised["shader"]["frag"] };
		sources.geometry.location = std::string{ serialised["shader"]["geom"] };
		if (!(*targetPtr)) *targetPtr = new Model("Model", sources, serialised["modelPath"]);

		Node *node = *targetPtr;
		Node::deserialise(serialised, &node);
	}


	Model::Model(const std::string &name, ShaderSources &sources, const fs::path &modelPath)
		: Node(name), IMeshContainer(sources)
	{
		m_type = "Model";
		setModelPath(modelPath);
	}


	void Model::setModelPath(const fs::path &path)
	{
		m_meshes.clear();
		std::string modelPath = path.string();

		// Replace all backslashes with forward slashes
		std::replace(modelPath.begin(), modelPath.end(), '\\', '/');

		// Find the last forward slash, to distinguish between directory and filename
		size_t indexOfLastSlash = modelPath.find_last_of("/");
		m_modelDirectory = modelPath.substr(0, indexOfLastSlash);
		m_modelFile = modelPath.substr(indexOfLastSlash + 1, (modelPath.length() - indexOfLastSlash) + 1);

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			TE_CORE_ERROR(std::format("ASSIMP: {}", importer.GetErrorString()));
			return;
		}

		processNode(scene->mRootNode, scene);
	}


	void Model::processNode(aiNode *node, const aiScene *scene)
	{
		// Process all of node's meshes
		for (unsigned i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(std::move(processMesh(mesh, scene)));
		}

		// Recurse on child meshes
		for (unsigned i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}


	std::unique_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene)
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

		return std::make_unique<Mesh>(vertices, indices, textures);
	}


	std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, int assimpTexType, std::string typeName)
	{
		aiTextureType type = (aiTextureType)assimpTexType;

		std::vector<std::shared_ptr<Texture>> textures;

		for (unsigned i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skipLoading = false;

			// See if texture with same path has already been loaded. If it has, copy existing version.
			std::vector<std::shared_ptr<Texture>> loadedTextures = IShaderContainer::getLoadedTextures();
			for (unsigned j = 0; j < loadedTextures.size(); j++)
			{
				std::shared_ptr<Texture> loadedTex = loadedTextures[j];
				if (loadedTex->getPath().filename() == str.C_Str())
				{
					textures.push_back(loadedTex);
					skipLoading = true;
					break;
				}
			}

			if (!skipLoading)
			{
				auto tex = Texture::fromFile(m_modelDirectory / str.C_Str(), typeName);

				if (tex.has_value())
				{
					std::shared_ptr<Texture> val = tex.value();
					textures.push_back(val);
					IShaderContainer::addLoadedTexture(val);
				}
				else
				{
					TE_CORE_ERROR(std::format("Unable to load texture {}/{}", m_modelDirectory.string(), str.C_Str()));
				}
			}
		}

		return textures;
	}


	void Model::draw()
	{
		if (!getVisibility()) return;

		IOutlined::predraw();
		m_shader->use();

		m_shader->setVec3("tex_scale", glm::vec3{ 1, 1, 1 });
		m_shader->setFloat("material.Ns", 32.0f);

		auto cam = Scene::getActiveScene()->getActiveCamera();
		auto P = cam->getProj();
		auto V = cam->getView();
		auto M = Node::getTransform()->getWorldModelMatrix();
		auto VM = V * M;
		m_shader->setMat4("PVM", P * VM);
		m_shader->setMat4("VM", VM);
		m_shader->setMat4("V", V);
		m_shader->setMat4("VM_it", glm::inverseTranspose(VM));
		processLights();

		for (unsigned i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i]->draw(m_shader.get());
		}
		m_shader->unuse();
		IOutlined::postdraw(m_transform.get());
	}


	void Model::processLights()
	{
		auto scene = Scene::getActiveScene();
		auto activeLights = scene->getLights();
		for (Light *light : activeLights)
		{
			light->updateShader(m_shader.get());
		}

		m_shader->setInt("num_dir_lights", scene->getNumLights(LightType::Directional));
		m_shader->setInt("num_point_lights", scene->getNumLights(LightType::Point));
	}


	void Model::update()
	{
		Node::update();
	}
}
