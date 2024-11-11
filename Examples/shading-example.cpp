// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include "../Headers/oglopp.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


using namespace oglopp;

#define CAMSPEED (0.05f)

void handleInput(Window& window) {
	// When escape is pressed...
	if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		// Release the cursor
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// If control is also pressed.. Destroy the window
		if (glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			window.destroy();
		}
	}

	bool eventRecevied = false;
	if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
		window.getCam().translate(window.getCam().getBack() * -CAMSPEED);
		eventRecevied = true;
	}
	if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
		window.getCam().translate(window.getCam().getRight() * -CAMSPEED);
		eventRecevied = true;
	}
	if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
		window.getCam().translate(window.getCam().getBack() * CAMSPEED);
		eventRecevied = true;
	}
	if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
		window.getCam().translate(window.getCam().getRight() * CAMSPEED);
		eventRecevied = true;
	}
	if (glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		window.getCam().translate(window.getCam().getUp() * -CAMSPEED);
		eventRecevied = true;
	}
	if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
		window.getCam().translate(window.getCam().getUp() * CAMSPEED);
		eventRecevied = true;
	}

	// The cursor is trapped. So we can do window stuff now
	if (glfwGetInputMode(window.getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		double cursorX = 0;
		double cursorY = 0;
		glfwGetCursorPos(window.getWindow(), &cursorX, &cursorY);
		glfwSetCursorPos(window.getWindow(), 0, 0);

		glm::vec3 angle = window.getCam().getAngle();

		cursorX *= 0.04;
		cursorY *= 0.04;

		angle.y += cursorX;
		angle.x += cursorY;

		
		if (angle.z >= 89.0)
			angle.z = 89.0;
		if (angle.z <= -89.0)
			angle.z = -89.0;

		window.getCam().setAngle(angle);

	}

	if (eventRecevied) {
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window.getWindow(), 0, 0);
	}

	if (glfwGetKey(window.getWindow(), GLFW_KEY_R) == GLFW_PRESS) {
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// When cursor enters.. Trap it
}

int main() {

	// Create the window
	Window window;
	window.create(800, 800, "HoneyLib OpenGL - Shading");

	int width = 0;
	int height = 0;
	window.getSize(&width, &height);
	std::cout << "Window size: [" << width << ", " << height << "]" << std::endl;

	// Initialize the shape we want to draw
	Cube coob;
	Cube coob2;
	Cube coob3;

	coob.scale(glm::vec3(0.5));
	coob2.scale(glm::vec3(0.5, 0.5, 0.6));
	coob3.scale(glm::vec3(3.0));

	// // Initialize our shader object
	Shader ourShader(
		"#version 330 core\n"\
		"layout (location = 0) in vec3 aPos;\n"\
		"layout (location = 1) in vec3 aColor;\n"\
		"layout (location = 2) in vec2 aTexCoord;\n"\
		\
		"uniform mat4 model;\n"\
		"uniform mat4 view;\n"\
		"uniform mat4 projection;\n"\
		"uniform vec4 objectCol;\n"\
		\
		"out vec4 vertexColor;\n"\
		"out vec2 texCoord;\n"\
		"out vec4 objCol;\n"\
		\
		"void main() {\n"\
			"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"\
			"vertexColor = vec4(aColor, 1.0);\n"\
			"texCoord = aTexCoord;\n"\
			"objCol = objectCol;\n"\
		"}\n",


		"#version 330 core\n"\
		"out vec4 FragColor;\n"\
		"in vec4 objCol;\n"\
		\
		"uniform sampler2D texture1;\n"\
		"uniform sampler2D texture2;\n"\
		\
		"void main() {\n"\
			"float ambientStrength = 0.1;\n"\
			"FragColor = vec4(0.1) + objCol;\n"\
		"}\n", 

		ShaderType::RAW); 

	// Camera cam;
	float angle = 0;

	glEnable(GL_DEPTH_TEST);

	window.getCam().setPos(glm::vec3(0.0, 0.0, -4.0)).setAngle(glm::vec3(00, -90, 0));
	window.getCam().setFov(65);

	// ----- Render Loop -----
	while (!window.shouldClose()) {
		// Process events
		handleInput(window);
		
		
		angle += 0.02;

		//window.getCam().setAngle(glm::vec3(angle * 10, 0, 0.0));

		// Uniforms
		coob.rotate(glm::vec3(0.01));
		
		coob2.setPosition(glm::vec3(sin(angle), cos(angle), 0.0));
		
		coob3.setPosition(glm::vec3(sin(angle / 10) * 4, cos(angle / 10) * 4, cos(angle / 10) * sin(angle / 10) * 4));
		//coob2.translate(glm::vec3(0, 1.0, 0.0));

		//window.getCam().setPos(glm::vec3(sin(angle) * 4, 3.0, cos(angle) * 4));

		// Prepare render layer
		ourShader.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.setVec4("objectCol", glm::vec4(1.0, 0.5, 0.0, 1.0));
		coob.draw(window, &ourShader);

		ourShader.setVec4("objectCol", glm::vec4(1.0));
		coob2.draw(window, &ourShader);
		
		coob3.draw(window, &ourShader);

		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen (v-sync?)
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
