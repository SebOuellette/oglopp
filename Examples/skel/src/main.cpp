// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include <oglopp.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace oglopp;

#define CAMSPEED (0.05)

int main() {
	// Create the window
	Window window;
	window.create(800, 800, "HoneyLib OpenGL - Perspective Example");

	int width = 0;
	int height = 0;
	window.getSize(&width, &height);
	std::cout << "Window size: [" << width << ", " << height << "]" << std::endl;

	// Initialize the shape we want to draw
	//Shape triangle;
	Rectangle rect;
	Triangle tri;
	Cube coob;
	Cube coob2;

	coob.scale(glm::vec3(0.5));
	coob2.scale(glm::vec3(0.5, 0.5, 1.0));

	// // Initialize our shader object
	Shader ourShader("./shaders/vertex.glsl", "./shaders/fragment.glsl", ShaderType::FILE);

	// Camera cam;
	float angle = 0;

	Texture container("./assets/textures/container.jpg", oglopp::Texture::JPG);
	Texture face("./assets/textures/awesomeface.png", oglopp::Texture::PNG);

	tri.pushTexture(&face);
	rect.pushTexture(&container);
	rect.pushTexture(&face);
	coob.pushTexture(&container);
	coob.pushTexture(&face);
	coob2.pushTexture(&face);

	ourShader.use();
	ourShader.setVec4("ourColor", {0.0, 0.0, 0.0, 0.0});

	window.getCam().setPos(glm::vec3(0.0, 0.0, -4.0)).setAngle(glm::vec3(00, -90, 0));
	timespec time;

	// ----- Render Loop -----
	while (!window.shouldClose()) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &time);
		unsigned long long int fullTime = time.tv_sec * 1000000 + time.tv_nsec / 1000;
		angle = static_cast<double>(fullTime) / 5000000 * M_PI * 2;

		// Process events
		window.handleNoclip();

		// Update the projection and view matrices for all the shapes to be drawn
		int width, height;
		window.getSize(&width, &height);
		window.getCam().updateProjectionView(width, height);

		// Transform objects
		coob.setAngle(glm::dvec3(angle));
		coob2.setPosition(glm::vec3(sin(angle), cos(angle), 0.0));

		//Rendering
		window.clear();
		rect.draw(window, &ourShader);
		tri.draw(window, &ourShader);
		coob.draw(window, &ourShader);
		coob2.draw(window, &ourShader);

		// Swap buffers since we always draw on the back buffer instead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen (v-sync?)
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
