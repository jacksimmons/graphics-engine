#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix; // inversetranspose(model)


void main()
{
    Normal = normalMatrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    gl_Position = proj * view * vec4(FragPos, 1.0);
}