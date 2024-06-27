#version 330 core
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;

#define NUM_DIR_LIGHTS 4
uniform DirLight dirLights[NUM_DIR_LIGHTS];

#define NUM_POINT_LIGHTS 4
uniform PointLight pointLights[NUM_POINT_LIGHTS];

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 objectCol;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColour;


vec3 calcDirLight(DirLight light, vec3 normal, vec3 dirToView);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 dirToView);


void main()
{
    // Properties
    vec3 norm = normalize(Normal);
    vec3 dirToView = normalize(viewPos - FragPos);

    vec3 result;

    // Directional lights
    for (int i = 0; i < NUM_DIR_LIGHTS; i++)
    {
        result += calcDirLight(dirLights[i], norm, dirToView);
    }

    // Point lights
    for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        result += calcPointLight(pointLights[i], norm, FragPos, dirToView);
    }

    FragColour = vec4(result, 1.0);
}


vec3 calcDirLight(DirLight light, vec3 normal, vec3 dirToView)
{
    vec3 dirToLight = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    float diff = max(dot(normal, dirToLight), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular
    vec3 reflectDir = reflect(-dirToLight, normal);
    float spec = pow(max(dot(dirToView, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // Combine and return
    return (ambient + diffuse + specular);
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 dirToView)
{
    vec3 dirToLight = normalize(light.position - fragPos);

    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    float diff = max(dot(normal, dirToLight), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular
    vec3 reflectDir = reflect(-dirToView, normal);
    float spec = pow(max(dot(dirToView, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // Attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 /
    (light.constant
    + light.linear * dist
    + light.quadratic * (dist * dist));

    return (ambient + diffuse + specular) * attenuation;
}