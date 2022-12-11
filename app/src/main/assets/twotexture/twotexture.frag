#version 300 es
precision mediump float;
in vec2 v_texCoord;
out vec4 outColor;
uniform sampler2D lyfId;
uniform sampler2D nsId;

void main() {
    //如果第三个值是0.0，它会返回第一个输入；
    //如果是1.0，会返回第二个输入值。
    //0.2会返回80%的第一个输入颜色和20%的第二个输入颜色，即返回两个纹理的混合色。
    outColor = mix(texture(lyfId, v_texCoord), texture(nsId, v_texCoord), 0.2);
}