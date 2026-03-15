#version 450 core
in vec4 vertCol;
in vec2 texPos;

out vec4 FragColor;

uniform sampler2D texture0;

vec4 colAt(vec2 pos) {
    return texture(texture0, vec2(-pos.x, pos.y)).rgba;
}

void main() {
    FragColor = colAt(texPos);
}
