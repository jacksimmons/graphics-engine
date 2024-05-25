#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "Shape.hpp"
#include "Error.hpp"


Shape::Shape()
{
}


Shape::~Shape()
{
	glDisableVertexAttribArray(0);
	delete vertices;
}


void Shape::init()
{
	glGenBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,							// (Attribute 0)
		numVertices,				// Size
		GL_FLOAT,					// Type
		GL_FALSE,					// Normalised?
		numVertices * sizeof(float),// Stride
		(void *)0					// Array buffer offset
	);
	glEnableVertexAttribArray(0);
}


void Shape::draw()
{
	glDrawArrays(mode, 0, numVertices);
}


Tri::Tri() : Shape()
{
	mode = GL_TRIANGLES;
	numVertices = 3;

	vertices = new GLfloat[numVertices]
	{
		-1.0f,		-1.0f,		0.0f,
		1.0f,		-1.0f,		0.0f,
		0.0f,		1.0f,		0.0f,
	};

	init();
}