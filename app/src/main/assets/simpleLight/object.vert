#version 300 es
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 fragPos;

void main() {
    normal = a_normal;
    fragPos = vec3(model * vec4(a_position, 1.0));
    gl_Position = projection * view * vec4(fragPos, 1.0);
}