// https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>
#include <cmath>
#include "../Headers/shader.hpp"
#include "../Headers/shape.hpp"
#include "../Headers/rectangle.hpp"
#include "../Headers/window.hpp"

int main() {
	// Create the window
	Window window;
	window.create();

	// Initialize the shape we want to draw
	//Shape triangle;
	Rectangle rect;

	// Initialize our shader object
	Shader ourShader("Shaders/multiColour.vert", "Shaders/multiColour.frag");

	// ----- Render Loop -----
	while (!window.shouldClose()) {
		// Process events
		window.processInput();
		glClear(GL_COLOR_BUFFER_BIT);

		// Uniforms
		float alphaValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;
		

		ourShader.setVec4("ourColor", {0.0, 0.0, 0.0, alphaValue});
		ourShader.use();

		// Rendering

		// Draw Triangle
		//triangle.draw();

		rect.draw();


		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
