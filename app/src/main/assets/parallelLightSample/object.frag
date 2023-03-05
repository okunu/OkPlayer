#version 300 es
precision mediump float;

//之前是要每个分量乘以物体颜色，这里相当于为每个分量定义一个颜色
struct Material {
    sampler2D  diffuse;
    sampler2D  specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
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
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    if(theta > light.cutOff) {
        float distance = length(light.position - fragPos);
        float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * distance * distance);

        vec3 ambient = light.ambient * (texture(material.diffuse, texCoords)).rgb;
//        ambient = ambient * attenuation;

        vec3 norm = normalize(normal);

        //如果传的是方向值 ，就直接计算光方向的单位向量
        //    vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * (texture(material.diffuse, texCoords)).rgb;
//        diffuse = diffuse * attenuation;

        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir =  reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * (texture(material.specular, texCoords)).rgb;
//        specular = specular * attenuation;

        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    } else {
        vec3 ambient = light.ambient * (texture(material.diffuse, texCoords)).rgb;
        FragColor = vec4(ambient, 1.0);
    }


}