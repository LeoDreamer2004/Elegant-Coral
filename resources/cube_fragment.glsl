#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;// 切光角
    float outerCutOff;// 外切光角

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec3 objColor;

void main() {
    vec3 matAmbient = vec3(texture(material.diffuse, TexCoords));
    vec3 matDiffuse = matAmbient;
    vec3 matSpecular = vec3(texture(material.specular, TexCoords));

    vec3 lightDir = normalize(light.position - FragPos);

    // 环境光照
    vec3 ambient = matAmbient * light.ambient;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = clamp((theta - light.outerCutOff) / (light.cutOff - light.outerCutOff),
    0.0, 1.0);// 用于柔化边缘, 用两层线性过渡

    // 漫反射
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * matDiffuse * light.diffuse * epsilon;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = matSpecular * spec * light.specular * epsilon;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
