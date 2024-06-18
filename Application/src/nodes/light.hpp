#include <glm/glm.hpp>
#include "nodes/model.hpp"


namespace Tank
{
	class Shader;
	class Light : public Node
	{
	private:
	protected:
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		int m_index;
		// Name of the array in GLSL containing all light structs.
		std::string m_lightArrayName;
		Light(std::string name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
	public:
		virtual void updateShader(Shader *) = 0;
		std::string getLightStruct() { return m_lightArrayName + "[" + std::to_string(m_index) + "]"; };
	};


	class DirLight : public Light
	{
	private:
		glm::vec3 m_direction;
		static int s_count;
	public:
		DirLight(std::string name, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);

		void updateShader(Shader *) override;
	};


	class PointLight : public Light
	{
	private:
		glm::vec3 m_position;
		static int s_count;
	public:
		PointLight(std::string name, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);

		void updateShader(Shader *) override;
	};
}