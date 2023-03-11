#version 300 es
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_txtcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main() {
    normal = mat3(transpose(inverse(model))) * a_normal;
    texCoords = vec2(a_txtcoords.x, 1.0 - a_txtcoords.y);
    fragPos = vec3(model * vec4(a_position, 1.0));
    gl_Position = projection * view * vec4(fragPos, 1.0);
}