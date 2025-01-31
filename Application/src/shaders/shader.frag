#version 460 core
struct Material
{
    sampler2D Kd;
    sampler2D Ks;
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

#define NUM_DIR_LIGHTS 1
uniform DirLight dirLights[NUM_DIR_LIGHTS];

#define NUM_POINT_LIGHTS 1
uniform PointLight pointLights[NUM_POINT_LIGHTS];

uniform vec3 tex_scale;
uniform mat4 VM;

in vec3 frag_normal;
in vec3 frag_pos;
in vec2 frag_tex_coords;

out vec4 frag_colour;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 dirToView);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 frag_pos);


void main()
{
    // Properties
    vec3 norm_normal = normalize(frag_normal);
    
    vec3 result;
    // Directional lights
//    for (int i = 0; i < NUM_DIR_LIGHTS; i++)
//    {
//        result += calcDirLight(dirLights[i], norm, dirToView);
//    }

    // Point lights
    for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        result += calcPointLight(pointLights[i], norm_normal, frag_pos);
    }

    frag_colour = vec4(result, 1.0);
}


vec3 calcDirLight(DirLight light, vec3 normal, vec3 dirToView)
{
//    vec3 dirToLight = normalize(-light.direction);
//
//    // Ambient
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//
//    // Diffuse
//    float diff = max(dot(normal, dirToLight), 0.0);
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//
//    // Specular
//    vec3 reflectDir = reflect(-dirToLight, normal);
//    float spec = pow(max(dot(dirToView, reflectDir), 0.0), material.shininess);
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//
//    // Combine and return
//    return (ambient + diffuse + specular);

    return vec3(0.0f);
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 frag_pos)
{
    vec3 light_dir = normalize((VM * vec4(light.pos, 1.0f)).xyz - frag_pos);
    vec3 camera_dir = -normalize(frag_pos);

    // === Ambient Reflection
    vec3 ambient = light.Ia * vec3(texture(material.Kd, frag_tex_coords * tex_scale.xy));

    // === Diffuse Reflection
    // A . B = |A||B|cos(theta)
    // |normal| and |light_dir| are 1
    // => diff = cos(theta)
    // If the angle > 90 deg, then dot product becomes negative.
    // max function ensures negative lighting does not occur (which is undefined).
    float angle_normal_light = max(dot(normal, light_dir), 0.0f);
    vec3 diffuse = light.Id * vec3(texture(material.Kd, frag_tex_coords * tex_scale.xy)) * angle_normal_light;

    // === Specular Reflection
    // Need to negate light_dir, as it points toward the light.
    // The `reflect` function expects a vector pointing out from
    // the light source. It is then reflected over the normal.
    float spec = 0.0f;
    int blinn = 1;
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

    vec3 specular = light.Is * vec3(texture(material.Ks, frag_tex_coords * tex_scale.xy)) * spec;
    
    // Overwrite specular component if exponent is 0
    if (material.Ns == 0) {
        specular = vec3(0.0f);
    }

    // Attenuation
    float dist = length(light.pos - frag_pos);
    float attenuation = 1.0 /
    (light.constant
    + light.linear * dist
    + light.quadratic * (dist * dist));

    return (ambient + diffuse + specular) * attenuation;
}