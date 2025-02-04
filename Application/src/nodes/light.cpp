#include <string>
#include "transform.hpp"
#include "shader.hpp"
#include "log.hpp"
#include "nodes/light.hpp"
#include "nodes/scene.hpp"


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


	void Light::updateShader(Shader *shader)
	{
		std::string str = getLightStruct();
		if (getEnabled())
		{
			shader->setVec3(str + ".Ia", m_ambient);
			shader->setVec3(str + ".Id", m_diffuse);
			shader->setVec3(str + ".Is", m_specular);
		}
		else
		{
			shader->setVec3(str + ".Ia", glm::vec3(0.0f));
			shader->setVec3(str + ".Id", glm::vec3(0.0f));
			shader->setVec3(str + ".Is", glm::vec3(0.0f));
		}
	}


	DirLight::DirLight(const std::string &name, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: Light(name, amb, diff, spec), m_direction(dir)
	{
		m_index = DirLight::s_count;
		m_lightArrayName = "dirLights";

		if (DirLight::s_count >= 64)
		{
			TE_CORE_WARN("Reached limit of DirLight lights; this light will not apply to shaders.");
			return;
		}

		DirLight::s_count++;
		m_scene->addLight(this);
	}


	DirLight::~DirLight()
	{
		DirLight::s_count--;
	}


	void DirLight::updateShader(Shader *shader)
	{
		std::string str = getLightStruct();
		shader->setVec3(str + ".dir", m_direction);

		Light::updateShader(shader);
	}


	PointLight::PointLight(const std::string &name, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: Light(name, amb, diff, spec)
	{
		m_index = PointLight::s_count;
		m_lightArrayName = "pointLights";

		if (PointLight::s_count >= 64)
		{
			TE_CORE_WARN("Reached limit of PointLight lights; this light will not apply to shaders.");
			return;
		}

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
		shader->setVec3(str + ".pos", Mat4::getTranslation(getTransform()->getWorldModelMatrix()));
		shader->setFloat(str + ".constant", 1.0f);
		shader->setFloat(str + ".linear", 0.0f);
		shader->setFloat(str + ".quadratic", 0.0f);

		Light::updateShader(shader);
	}
}