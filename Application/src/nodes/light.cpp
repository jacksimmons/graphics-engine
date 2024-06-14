#include "transform.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	Light::Light(std::string name, glm::vec3 pos) : Node("LightSource")
	{
		auto lightCube = std::make_unique<Model>("LightCube", "lightCubeShader.vert", "lightCubeShader.frag");
		lightCube->getTransform()->setScale({ 0.1f, 0.1f, 0.1f });
		lightCube->getTransform()->setTranslation(pos);
		addChild(std::move(lightCube));
	}
}