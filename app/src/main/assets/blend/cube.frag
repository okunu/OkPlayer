#version 300 es
precision mediump float;
in vec2 v_texCoord;
out vec4 outColor;
uniform sampler2D firId;

void main() {
    outColor = texture(firId, v_texCoord);
}