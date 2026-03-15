#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec2 texCoord;

void main() {
    FragColor = vec4(Normal, 1.0); // + texture(texture0, texCoord) + texture(texture1, texCoord)) / 3.0;
    FragColor.a = 1.0;
}
