#version 330 core
precision mediump float;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 rotation;

in VS_OUT {
    vec3 vNormal;
    vec2 vTexCoord;
    float vOption;
} gs_in[];

out vec3 FragPos;
out vec3 Normal;
out vec2 texCoord;
out float Option;

void main() {
    for (int i = 0; i < 3; i++) {
        vec4 scale = vec4(1.0, 1.0, 1.0, 1.0);

        vec3 norm = gl_in[i].gl_Position.xyz;
        vec4 newPos = (scale * vec4(norm, 1.0));
        vec4 newNormal = (rotation * scale * vec4(gs_in[i].vNormal, 1.0));

        gl_Position = projection * view * model * newPos;
        Normal = newNormal.xyz;
        texCoord = gs_in[i].vTexCoord;
        FragPos = vec3(model * newPos);
        Option = gs_in[i].vOption - 0.1;
        EmitVertex();
    }
    EndPrimitive();
}
