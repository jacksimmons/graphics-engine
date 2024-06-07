#include <memory>
#include <vector>
#include <glm/glm.hpp>


class Shape
{
protected:
	GLuint vertexBuffer;
	GLfloat *vertices;
	int mode = GL_TRIANGLES;
	int numVertices = 0;
public:
	Shape();
	~Shape();
	void init();
	void draw();
};


class Tri : public Shape
{
public:
	Tri();
};