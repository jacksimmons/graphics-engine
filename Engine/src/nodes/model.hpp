#pragma once

#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "interfaces/mesh_container.hpp"


struct aiNode; struct aiScene; struct aiMesh;
struct aiMaterial;
namespace Tank
{
	class Texture;

	/// <summary>
	/// A class which can load 3D models using Assimp.
	/// </summary>
	class TANK_API Model : public Node, public IMeshContainer
	{
	public:
		virtual json serialise() override;
		static void deserialise(const json &serialised, Model **targetPtr);

	private:
		fs::path m_modelDirectory;
		fs::path m_modelFile;
	public:
		Model(const std::string &name,
			ShaderSources &sources,
			const fs::path &modelPath
		);
		virtual ~Model() = default;

		void setModelPath(const fs::path &path);
		fs::path getModelPath() const { return m_modelDirectory / m_modelFile; }
		virtual void draw() override;
		virtual void update() override;
		void processNode(aiNode *node, const aiScene *scene);
		std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
		void processLights();
		
		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, int assimpTextureType, std::string typeName);
	};
}
