#version 300 es
out vec4 fragColor;
uniform vec4 outColor;
void main(){
    fragColor = outColor;
//    fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );
}