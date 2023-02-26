#version 300 es
precision mediump float;

//之前是要每个分量乘以物体颜色，这里相当于为每个分量定义一个颜色
struct Material {
    sampler2D  diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 FragColor;

void main()
{
    vec3 ambient = light.ambient * (texture(material.diffuse, texCoords)).rgb;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * (texture(material.diffuse, texCoords)).rgb;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir =  reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}