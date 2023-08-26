// https://learnopengl.com

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "../Headers/shader.hpp"
#include "../Headers/shape.hpp"
#include "../Headers/rectangle.hpp"


// Callback function to automatically change viewport when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Event processing
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
}

int main() {
	// Instantiate the window
	//  Load up OpenGL version 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// This tells the compiler we want to use the core-profile; meaning a smaller subset of OpenGL features without backwards compatability features we don't need
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAR, GL_TRUE); // Needed on Mac OS
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD before we call any OpenGL functions
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}

	// Set the viewport
	glViewport(0, 0, 800, 600);

	// Use wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Callback function to automatically change viewport when window is resized
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	// Set the "background" colour of the window
	glClearColor(0.0, 0.0, 0.0, 0.0);


	// Initialize the shape we want to draw
	Shape triangle;

	// ----- Element Buffer Objects -----
	//  Traditionally, this is what we'd have to do to draw a rectangle
		// float vertices[] = {
		// 	// first triangle
		// 	0.5f,  0.5f, 0.0f,  // top right
		// 	0.5f, -0.5f, 0.0f,  // bottom right
		// 	-0.5f,  0.5f, 0.0f,  // top left 
		// 	// second triangle
		// 	0.5f, -0.5f, 0.0f,  // bottom right
		// 	-0.5f, -0.5f, 0.0f,  // bottom left
		// 	-0.5f,  0.5f, 0.0f   // top left
		// };
	// 50% overhead, redefining points

	//Rectangle rectangle;

	float vertices[] = {
		 0.5, -0.5,  0.0,	1.0, 0.0, 0.0, // Bottom left
		-0.5, -0.5,  0.0,	0.0, 1.0, 0.0, // Bottom right
		 0.0,  0.5,  0.0,	0.0, 0.0, 1.0  // Top
	};

	// Create an empty vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Bind the newly created buffer to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the vertex array data into the buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	// ----- Vertex Array Object -----
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Set the colour attribute	// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
	

	Shader ourShader("Shaders/multiColour.vert", "Shaders/multiColour.frag");

	// ----- Render Loop -----
	while (!glfwWindowShouldClose(window)) {
		// Process events
		processInput(window);

		// Uniforms
		float alphaValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;

		ourShader.setVec4("ourColor", {0.0, 0.0, 0.0, alphaValue});
		ourShader.use();

		// Rendering
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Rectangle
		//rectangle.draw();

		// Draw Triangle
		//triangle.draw();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glBegin( GL_TRIANGLES );
		// 	glVertex3f( 0.0f, 1.0f, 0.0f );
		// 	glVertex3f( -1.0f,-1.0f, 0.0f );
		// 	glVertex3f( 1.0f,-1.0f, 0.0f);
   		// glEnd();


		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clear all resources
	glfwTerminate();

	return 0;
}
