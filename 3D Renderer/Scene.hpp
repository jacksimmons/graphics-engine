#include <vector>
#include <memory>

#include "Model.hpp"


class Scene
{
public:
	bool running;
	int width, height;

	Scene(int width, int height);
	~Scene();

	void draw();
	void addModel(std::unique_ptr<Model>);
private:
	std::vector<std::unique_ptr<Model>> m_models;

};