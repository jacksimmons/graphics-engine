#version 330 core
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 objectCol;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColour;


void main()
{
    // Ambient
    float ambientStr = 0.1;
    vec3 ambient = ambientStr * lightCol;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 dirToLight = normalize(lightPos - FragPos);
    float diff = max(dot(norm, dirToLight), 0.0);
    vec3 diffuse = diff * lightCol;

    // Specular
    float specularStr = 0.5;
    vec3 dirToView = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-dirToView, norm); // Reflect view->frag ray about norm
    float spec = pow(max(dot(dirToView, reflectDir), 0.0), 32);
    vec3 specular = specularStr * spec * lightCol;

    //vec3 texCol = mix(texture(tex0, texCoord), texture(tex1, texCoord), 0.75).xyz;
    // Result
    FragColour = vec4((ambient + diffuse + specular) * objectCol, 1.0);
}