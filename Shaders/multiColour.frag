#version 140
		
out vec4 FragColor;
in vec4 vertexColor;

uniform vec4 ourColor;

void main() {
	vec4 colour = ourColor;
	if (gl_FragCoord.x > 400.0) {
		colour = vertexColor;
	}

	FragColor = colour; //vec4(1.0f, 0.5f, 0.2f, 1.0f);
}