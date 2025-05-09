// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include "../Headers/oglopp.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <ctime>
#include <iomanip>
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
	Shader ourShader(
		"#version 330 core\n"\
		"layout (location = 0) in vec3 aPos;\n"\
		"layout (location = 1) in vec3 aColor;\n"\
		"layout (location = 2) in vec2 aTexCoord;\n"\
		\
		"uniform mat4 model;\n"\
		"uniform mat4 view;\n"\
		"uniform mat4 projection;"\
		\
		"out vec4 vertexColor;\n"\
		"out vec2 texCoord;\n"\
		\
		"void main() {\n"\
			"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"\
			"vertexColor = vec4(aColor, 1.0);\n"\
			"texCoord = aTexCoord;\n"\
		"}\n",

		"#version 330 core\n"\
		"out vec4 FragColor;\n"\
		"in vec4 vertexColor;\n"\
		"in vec2 texCoord;\n"\
		\
		"uniform sampler2D texture1;\n"\
		"uniform sampler2D texture2;\n"\
		\
		"void main() {\n"\
			"FragColor =  (vertexColor + texture(texture1, texCoord) + texture(texture2, texCoord)) / 3.0;\n"\
		"}\n",

		ShaderType::RAW);

	// Camera cam;
	float angle = 0;

	Texture container("/network/Programming/opengl/Examples/assets/container.jpg");
	Texture face("/network/Programming/opengl/Examples/assets/awesomeface.png", oglopp::Texture::PNG);

	tri.pushTexture(face);
	rect.pushTexture(container);
	rect.pushTexture(face);
	coob.pushTexture(container);
	coob.pushTexture(face);
	coob2.pushTexture(face);

	window.getCam().setPos(glm::vec3(0.0, 0.0, -4.0)).setAngle(glm::vec3(00, -90, 0));

	ourShader.setVec4("ourColor", {0.0, 0.0, 0.0, 0.0});

	//std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
	//std::chrono::high_resolution_clock::time_point thisFrameTime;
	//std::chrono::high_resolution_clock::duration frameTimeDiff;
	timespec time;

	// ----- Render Loop -----
	while (!window.shouldClose()) {
		//thisFrameTime = std::chrono::high_resolution_clock::now();
		//frameTimeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(thisFrameTime - lastFrameTime);
		//lastFrameTime = thisFrameTime;
		clock_gettime(CLOCK_MONOTONIC_RAW, &time);
		unsigned long long int fullTime = time.tv_sec * 1000000 + time.tv_nsec / 1000;

		// Process events
		window.handleNoclip();

		//angle += static_cast<double>(frameTimeDiff.count()) * 0.000000002; //0.02;
		angle = static_cast<double>(fullTime) / 5000000 * M_PI * 2;

		// Update the projection and view matrices for all the shapes to be drawn
		int width, height;
		window.getSize(&width, &height);
		window.getCam().updateProjectionView(width, height);

		// Transform objects
		//coob.rotate(glm::vec3(0.01));
		coob2.setPosition(glm::vec3(sin(angle), cos(angle), 0.0));

		//Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rect.draw(window, &ourShader);
		tri.draw(window, &ourShader);
		coob.draw(window, &ourShader);
		coob2.draw(window, &ourShader);

		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen (v-sync?)
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
