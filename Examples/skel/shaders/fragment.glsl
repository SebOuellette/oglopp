#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    FragColor = (vertexColor + texture(texture0, texCoord) + texture(texture1, texCoord)) / 3.0;
    FragColor.a = 1.0;
}
