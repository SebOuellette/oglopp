#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 vertCol;
out vec2 texPos;

void main() {
    gl_Position = vec4(-2 * aPos.x, 2 * aPos.y, 0.0, 1.0);
    vertCol = vec4(aColor, 1.0);
    texPos = aTexCoord;
}
