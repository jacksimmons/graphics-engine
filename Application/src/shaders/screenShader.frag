#version 330 core

uniform sampler2D screenTexture;

in vec2 texCoord;

out vec4 fragColour;


void main()
{
    //fragColour = vec4(vertColour, 1.0f);
    //fragColour = mix(texture(tex0, texCoord), texture(tex1, texCoord), 0.75f);
    fragColour = texture(screenTexture, texCoord);
}