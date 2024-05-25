#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Shape.hpp"


class Model
{
public:
	Model(glm::vec3 position, glm::vec3 rotation, float scale, glm::vec4 colour, std::unique_ptr<Shape> shape);
	Model(std::unique_ptr<Shape> shape);
	~Model();
	void draw();
private:
	std::unique_ptr<Shape> m_shape;
	GLfloat *m_position;
	GLfloat *m_rotation;
	float m_scale;
	GLfloat *m_colour;
};