#version 330 core
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 lightCol;
uniform vec3 objectCol;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColour;


void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 dirToLight = normalize(light.pos - FragPos);
    float diff = max(dot(norm, dirToLight), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular
    vec3 dirToView = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-dirToView, norm); // Reflect view->frag ray about norm
    float spec = pow(max(dot(dirToView, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //vec3 texCol = mix(texture(tex0, TexCoord), texture(tex1, TexCoord), 0.75).xyz;

    // Result
    FragColour = vec4(ambient + diffuse + specular, 1.0);
}