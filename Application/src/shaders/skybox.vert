#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 proj;
out vec3 texCoords;


void main()
{
	gl_Position = proj * view * vec4(aPos, 1);
	gl_Position.z = gl_Position.w * 0.999;
	texCoords = -aPos;
}
