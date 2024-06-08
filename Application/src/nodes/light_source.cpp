#include "transform.hpp"
#include "nodes/light_source.hpp"


namespace Tank
{
	LightSource::LightSource(std::string name) : Node("LightSource")
	{
		auto lightCube = std::make_unique<Model>("LightCube", "lightCubeShader.vert", "lightCubeShader.frag");
		lightCube->getTransform()->setScale({ 0.1f, 0.1f, 0.1f });
		lightCube->getTransform()->setTranslation({ 1,0,0 });
		addChild(std::move(lightCube));
	}
}