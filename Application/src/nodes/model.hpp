#pragma once
#include <memory>
#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "interfaces/ioutlined.hpp"


struct aiNode; struct aiScene; struct aiMesh;
struct aiMaterial; enum aiTextureType;
namespace Tank
{
	class Texture;

	namespace Editor { class _Inspector; }

	class Model : public IOutlined
	{
		// Allow Inspector to read shaders.
		friend class Editor::_Inspector;

	public:
		static json serialise(Model *model);
		static void deserialise(const json &serialised, Model **targetPtr);

	private:
		static std::vector<std::shared_ptr<Texture>> s_loadedTextures;
		std::string m_modelDirectory;
		std::string m_modelFile;
	protected:
		std::unique_ptr<Shader> m_shader;
	public:
		static const std::vector<std::shared_ptr<Texture>> &getLoadedTextures() { return s_loadedTextures; }

		Model(const std::string &name,
			const std::string &vsName,
			const std::string &fsName,
			const std::string &modelPath
		);

		void setShader(std::unique_ptr<Shader> shader) { m_shader = std::move(shader); };

		virtual void draw() override;
		virtual void update() override;
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);

		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		static void deleteLoadedTexture(Texture *texture)
		{
			int index = -1;
			for (unsigned int i = 0; i < s_loadedTextures.size(); i++)
			{
				if (s_loadedTextures[i].get() == texture)
					index = i;
			}

			if (index >= 0) s_loadedTextures.erase(s_loadedTextures.begin() + index);
		}
	};
}