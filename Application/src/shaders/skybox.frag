#version 460 core
// Direction which represents a 3D texture coordinate
in vec3 texCoords;
uniform samplerCube cubeMap;
out vec4 fragColour;


void main(void)
{
	fragColour = texture(cubeMap, texCoords);
}