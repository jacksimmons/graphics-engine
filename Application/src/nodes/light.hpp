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

		void setAmbient(glm::vec3 ambient) { m_ambient = ambient; }
		glm::vec3 getAmbient() const { return m_ambient; }
		
		void setDiffuse(glm::vec3 diffuse) { m_diffuse = diffuse; }
		glm::vec3 getDiffuse() const { return m_diffuse; }
		
		void setSpecular(glm::vec3 specular) { m_specular = specular; }
		glm::vec3 getSpecular() const { return m_specular; }
	};


	class DirLight : public Light
	{
	private:
		glm::vec3 m_direction;
		static int s_count;
	public:
		DirLight(std::string name, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);

		void updateShader(Shader *) override;

		void setDirection(glm::vec3 direction) { m_direction = direction; }
		glm::vec3 getDirection() const { return m_direction; }
	};


	class PointLight : public Light
	{
	private:
		static int s_count;
	public:
		PointLight(std::string name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);

		void updateShader(Shader *) override;
	};
}