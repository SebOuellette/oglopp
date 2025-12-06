#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aOption;

out VS_OUT {
    vec3 vNormal;
    vec2 vTexCoord;
    float vOption;
} vs_out;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vs_out.vTexCoord = aTexCoord;
    vs_out.vNormal = aNormal;
    vs_out.vOption = aOption;
}
