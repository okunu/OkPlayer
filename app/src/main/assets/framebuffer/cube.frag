#version 300 es
precision mediump float;
in vec2 v_texCoord;
out vec4 outColor;

uniform sampler2D cubeId;

void main() {
    outColor = texture(cubeId, v_texCoord);
}