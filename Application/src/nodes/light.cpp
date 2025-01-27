#include <string>
#include "transform.hpp"
#include "shader.hpp"
#include "scene.hpp"
#include "log.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	int DirLight::s_count = 0;
	int PointLight::s_count = 0;


	Light::Light(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) : Node(name),
		m_ambient(amb), m_diffuse(diff), m_specular(spec), m_index(0)
	{
		m_scene = Scene::getActiveScene();
	}


	Light::~Light()
	{
		TE_CORE_INFO("Destructor called");
		m_scene->removeLight(this);
	}


	DirLight::DirLight(const std::string &name, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: Light(name, amb, diff, spec), m_direction(dir)
	{
		m_index = DirLight::s_count;
		m_lightArrayName = "dirLights";
		DirLight::s_count++;

		m_scene->addLight(this);
	}


	DirLight::~DirLight()
	{
		TE_CORE_INFO("Dir Destructor called");

		DirLight::s_count--;
	}


	void DirLight::updateShader(Shader *shader)
	{
		std::string str = getLightStruct();
		shader->setVec3(str + ".ambient", m_ambient);
		shader->setVec3(str + ".diffuse", m_diffuse);
		shader->setVec3(str + ".specular", m_specular);
		shader->setVec3(str + ".direction", m_direction);
	}


	PointLight::PointLight(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: Light(name, amb, diff, spec)
	{
		m_index = PointLight::s_count;
		m_lightArrayName = "pointLights";
		PointLight::s_count++;

		m_scene->addLight(this);
	}


	PointLight::~PointLight()
	{
		TE_CORE_INFO("Pt Destructor called");

		PointLight::s_count--;
	}


	void PointLight::updateShader(Shader *shader)
	{
		std::string str = getLightStruct();
		shader->setVec3(str + ".ambient", m_ambient);
		shader->setVec3(str + ".diffuse", m_diffuse);
		shader->setVec3(str + ".specular", m_specular);
		shader->setVec3(str + ".position", getTransform()->getTranslation());
		shader->setFloat(str + ".constant", 1.0f);
		shader->setFloat(str + ".linear", 0.0f);
		shader->setFloat(str + ".quadratic", 0.0f);
	}
}