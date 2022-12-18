#version 300 es
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texCoord;
out vec2 v_texCoord;
uniform mat4 transform;

void main() {
    gl_Position = transform * a_position;
    v_texCoord = a_texCoord;
}