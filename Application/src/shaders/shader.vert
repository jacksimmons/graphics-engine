#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertColour;
out vec2 texCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;


void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    vertColour = aColour;
    texCoord = aTexCoord;
}