#pragma once
#include <memory>
#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "interfaces/outlined.hpp"
#include "interfaces/has_shader.hpp"


struct aiNode; struct aiScene; struct aiMesh;
struct aiMaterial; enum aiTextureType;
namespace Tank
{
	class Texture;

	namespace Editor { class _Inspector; }


	/// <summary>
	/// A class which can load 3D models using Assimp.
	/// </summary>
	class Model : public IOutlined, public IHasShader
	{
		// Allow Inspector to read shaders.
		friend class Editor::_Inspector;

	public:
		static json serialise(Model *model);
		static void deserialise(const json &serialised, Model **targetPtr);

	private:
		std::string m_modelDirectory;
		std::string m_modelFile;
	public:
		Model(const std::string &name,
			const std::string &vsName,
			const std::string &fsName,
			const std::string &modelPath
		);

		virtual void draw() override;
		virtual void update() override;
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);

		void loadMaterialTextures(std::vector<std::shared_ptr<Texture>> textures, aiMaterial *mat, aiTextureType type, std::string typeName);
	};
}