#version 300 es
precision mediump float;
in vec2 v_texCoord;
out vec4 outColor;

uniform sampler2D screenId;

void main() {
    outColor = vec4(vec3(1.0 - texture(screenId, v_texCoord)), 1.0);
}