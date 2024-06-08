#version 330 core
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 lightColour;

in vec3 vertColour;
in vec2 texCoord;

out vec4 fragColour;


void main()
{
    float ambientStr = 0.1;
    vec3 ambientCol = ambientStr * lightColour;
    vec3 texCol = mix(texture(tex0, texCoord), texture(tex1, texCoord), 0.75f).xyz;
    fragColour = vec4(texCol + ambientCol, 1.0);
}