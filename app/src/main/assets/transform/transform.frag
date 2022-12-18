#version 300 es
precision mediump float;
in vec2 v_texCoord;
out vec4 outColor;
uniform sampler2D lyfId;

void main() {
    outColor = texture(lyfId, v_texCoord);
}