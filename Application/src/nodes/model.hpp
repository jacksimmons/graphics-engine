#pragma once
#include <memory>
#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"


struct aiNode; struct aiScene; struct aiMesh;
struct aiMaterial; enum aiTextureType;
namespace Tank
{
	class Texture;
	class Model : public Node
	{
		friend class Inspector;
	private:
		static std::vector<Texture> s_texturesLoaded;
		std::vector<Mesh> m_meshes;
		std::string m_directory;
	protected:
		std::unique_ptr<Shader> m_shader;
	public:
		Model(const std::string &name,
			const std::string &vsName,
			const std::string &fsName,
			const std::string &modelPath = "models/bigben.obj"
		);

		void setShader(std::unique_ptr<Shader> shader) { m_shader = std::move(shader); };

		virtual void draw() override;
		virtual void update() override;
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	};
}