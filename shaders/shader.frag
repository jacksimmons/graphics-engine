#version 460 core
#define MAX_TEXTURES 16
struct Material
{
    sampler2D[MAX_TEXTURES] diffuse;
    sampler2D[MAX_TEXTURES] specular;
    float Ns;
};

struct DirLight
{
    vec3 Ia;
    vec3 Id;
    vec3 Is;

    vec3 dir;
};

struct PointLight
{
    vec3 Ia;
    vec3 Id;
    vec3 Is;

    vec3 pos;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;

#define MAX_LIGHTS 64
uniform int num_dir_lights;
uniform DirLight dirLights[MAX_LIGHTS];
uniform int num_point_lights;
uniform PointLight pointLights[MAX_LIGHTS];

uniform vec3 tex_scale;
uniform mat4 V;

in vec3 frag_normal;
in vec3 frag_pos;
in vec2 frag_tex_coords;

out vec4 frag_colour;

vec3 calcDirLight(DirLight light, vec3 normal);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 frag_pos);
vec3 calcLightAmbient(vec3 Ia);
vec3 calcLightDiffuse(vec3 Id, vec3 normal, vec3 light_dir);
vec3 calcLightSpecular(vec3 Is, float Ns, vec3 normal, vec3 light_dir, vec3 camera_dir);


void main()
{
    // Properties
    vec3 norm_normal = normalize(frag_normal);
    
    vec3 result;
    // Directional lights
    for (int i = 0; i < num_dir_lights; i++)
    {
        result += calcDirLight(dirLights[i], norm_normal);
    }

    // Point lights
    for (int i = 0; i < num_point_lights; i++)
    {
        result += calcPointLight(pointLights[i], norm_normal, frag_pos);
    }

    frag_colour = vec4(result, 1.0);
}



vec3 calcLightAmbient(vec3 Ia)
{
    vec3 ambient;
    for (int i = 0; i < MAX_TEXTURES; i++)
    {
        ambient += Ia * vec3(texture(material.diffuse[i], frag_tex_coords * tex_scale.xy));
    }
    return ambient;
}


vec3 calcLightDiffuse(vec3 Id, vec3 normal, vec3 light_dir)
{
    // A . B = |A||B|cos(theta)
    // |normal| and |light_dir| are 1
    // => diff = cos(theta)
    // If the angle > 90 deg, then dot product becomes negative.
    // max function ensures negative lighting does not occur (which is undefined).
    float angle_normal_light = max(dot(normal, light_dir), 0.0f);

    vec3 diffuse;
    for (int i = 0; i < MAX_TEXTURES; i++)
    {
        diffuse += Id * vec3(texture(material.diffuse[i], frag_tex_coords * tex_scale.xy)) * angle_normal_light;
    }

    return diffuse;
}


vec3 calcLightSpecular(vec3 Is, float Ns, vec3 normal, vec3 light_dir, vec3 camera_dir)
{
    // Need to negate light_dir, as it points toward the light.
    // The `reflect` function expects a vector pointing out from
    // the light source. It is then reflected over the normal.
    float spec = 0.0f;
    int blinn = 0;
    if (blinn == 0)
    {
        // Phong
        vec3 reflect_dir = reflect(-light_dir, normal);
        spec = pow(max(dot(camera_dir, reflect_dir), 0.0f), material.Ns);
    }
    else
    {
        // Blinn-Phong
        vec3 halfway_dir = normalize(light_dir + camera_dir);
        spec = pow(max(dot(normal, halfway_dir), 0.0f), material.Ns);
    }

    // Overwrite specular component if exponent is 0
    if (material.Ns == 0) {
        return vec3(0.0f);
    }

    vec3 specular;
    for (int i = 0; i < MAX_TEXTURES; i++)
    {
        specular += Is * vec3(texture(material.specular[i], frag_tex_coords * tex_scale.xy)) * spec;
    }
    return specular;
}


vec3 calcDirLight(DirLight light, vec3 normal)
{
    vec3 light_dir = normalize(mat3(V) * -light.dir);
    vec3 camera_dir = -normalize(frag_pos);

    vec3 ambient = calcLightAmbient(light.Ia);
    vec3 diffuse = calcLightDiffuse(light.Id, normal, light_dir);
    vec3 specular = calcLightSpecular(light.Is, material.Ns, normal, light_dir, camera_dir);
    return (ambient + diffuse + specular);
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 frag_pos)
{
    vec3 light_dir = normalize((V * vec4(light.pos, 1.0f)).xyz - frag_pos);
    vec3 camera_dir = -normalize(frag_pos);

    vec3 ambient = calcLightAmbient(light.Ia);
    vec3 diffuse = calcLightDiffuse(light.Id, normal, light_dir);
    vec3 specular = calcLightSpecular(light.Is, material.Ns, normal, light_dir, camera_dir);

    // Attenuation
    float dist = length(light.pos - frag_pos);
    float attenuation = 1.0 /
    (light.constant
    + light.linear * dist
    + light.quadratic * (dist * dist));

    return (ambient + diffuse + specular) * attenuation;
}