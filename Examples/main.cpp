// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <cmath>
#include "../Headers/oglopp.h"

using namespace oglopp;

int main() {

	// Create the window
	Window window;
	window.create();

	// Initialize the shape we want to draw
	//Shape triangle;
	Rectangle rect;
	Triangle tri;

	// // Initialize our shader object
	Shader ourShader(
		"#version 330 core\n"\
		"layout (location = 0) in vec3 aPos;\n"\
		"layout (location = 1) in vec3 aColor;\n"\
		"layout (location = 2) in vec2 aTexCoord;\n"\
		"uniform mat4 transform;\n"\
		"out vec4 vertexColor;\n"\
		"out vec2 texCoord;\n"\
		"void main() {\n"\
			"gl_Position = vec4(aPos, 1.0) * transform;\n"\
			"vertexColor = vec4(aColor, 1.0);\n"\
			"texCoord = aTexCoord;\n"\
		"}\n",

		"#version 140\n"\
		"out vec4 FragColor;\n"\
		"in vec4 vertexColor;\n"\
		"in vec2 texCoord;\n"\
		"uniform sampler2D texture1;\n"\
		"void main() {\n"\
			"FragColor = vertexColor + texture(texture1, texCoord);\n"\
		"}\n", ShaderType::RAW); 

	// Camera cam;
	glm::mat4 transform(1.f);

	Texture container;
	container.load("./container.jpg");

	tri.setTexture(&container);

	// ----- Render Loop -----
	while (!window.shouldClose()) {
		// Process events
		window.processInput();
		glClear(GL_COLOR_BUFFER_BIT);

		// Uniforms
		float alphaValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;

		transform = glm::rotate<float>(transform, 0.001, glm::vec3(0.0, 0.0, 1.0f));

		ourShader.setMat4("transform", &transform);
		ourShader.setVec4("ourColor", {0.0, 0.0, 0.0, alphaValue});
		ourShader.use();

		//Rendering
		rect.draw();
		tri.draw();


		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen (v-sync?)
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
