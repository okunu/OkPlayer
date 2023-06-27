#version 300 es
precision mediump float;
out vec4 outColor;

in vec3 normal;
in vec3 position;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main() {
    vec3 I = normalize(position - cameraPos);
    vec3 R = reflect(I, normalize(normal));
    outColor = vec4(texture(skybox, R).rgb, 1.0);
}