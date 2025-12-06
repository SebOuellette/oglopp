#version 330 core
struct Color {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    Color color;
    float shininess;
};

uniform Material material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D blockTex;
uniform sampler2D stoneTex;
uniform sampler2D dirtTex;
uniform sampler2D grassTex;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;
in float Option;

out vec4 FragColor;

void main() {
    vec3 albedo = material.color.diffuse;
    if (Option <= 0) { // Air
        albedo = vec3(0);
    } else if (Option <= 1) { // Stone
        albedo = texture(stoneTex, texCoord).xyz;
    } else if (Option <= 2) { // Dirt
        albedo = texture(dirtTex, texCoord).xyz;
    } else if (Option <= 3) { // Grass
        albedo = texture(grassTex, texCoord).xyz;
    } else if (Option <= 4) { // Water
        albedo = texture(blockTex, texCoord).xyz;
    }

    //albedo =

    vec3 ambient = lightColor * material.color.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * albedo); //norm.xyz);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.color.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0); //vec4(norm, 1.0); //vec4(result, 1.0);
}
