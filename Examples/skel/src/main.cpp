#include <chrono>
#include <cstddef>
#include <oglopp.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <filesystem>

#include "defines.h"
#include "oglopp/shader.h"
#include "shaders.h"

using namespace oglopp;

struct ResizeData {
	FBO* fbo;
	Texture* tex;
};

void resizeCallback(int width, int height, void* data) {
	if (data == nullptr) return; // Data may be null since we set it later on

	// Get the FBO pointer and resize it
	ResizeData* obj = static_cast<ResizeData*>(data);
	obj->tex->resizeWithFbo(*obj->fbo, width, height);
}

struct KeypressVars {
	bool keyPressed = false;
};

void keypressCallback(int key, int scancode, int action, int mods, void* data) {
	if (data == nullptr) return;
	if (action != GLFW_PRESS) return;

	KeypressVars* obj = static_cast<KeypressVars*>(data);

	switch (key) {
		case GLFW_KEY_SPACE:
			obj->keyPressed = !obj->keyPressed;
			break;

		default:
			break;
	}
}

int main() {
	// Setup some window options to make it invisible
	Window::Settings options;
	options.visible = true;
	options.modifyPointSize = true;
	options.clearColor = glm::vec4(glm::vec3(0), 0.0);
	options.resizeCallback = resizeCallback;
	options.keypressCallback = keypressCallback;

	// Create the window
	Window window;
	window.create(800, 800, "oglopp-example", options);
	//InputBuffer::windowPtr = &window;
	//glfwSetScrollCallback(window.getWindow(), InputBuffer::scrollCallback);

	int height = 0, width = 0;
	window.getSize(&width, &height); // Get the initial size for setting the size of the fbo

	Shaders shdrs;
	shdrs.view.setDrawType(DrawType::TRIANGLES);
	//glLineWidth(1.0f);
	shdrs.post.setDrawType(DrawType::TRIANGLES); // Canvas

	// -------------- FRAME BUFFER -----------
	// Create the FBO and texture objects
	FBO fbo(width, height);
	Texture fboTex(fbo, width, height);

	// Setup the resize callback data pointer
	ResizeData resizeData;
	resizeData.fbo = &fbo;
	resizeData.tex = &fboTex;
	window.setResizeCallbackDataPtr(&resizeData);

	// -------------- KEYPRESS CALLBACK ----------
	KeypressVars keypressVars;
	window.setKeypressCallbackDataPtr(&keypressVars);


	// -------------- OBJECTS ----------------
	//
	// Create a list of vertices with just an ID. the position will be provided in an SSBO calculated by a compute shader
	Cube cube;
	cube.setScale(glm::dvec3(1));

	// Create the canvas for drawing the framebuffer
	Rectangle canvas;
	canvas.pushTexture(&fboTex);


	// -------------------- SSBO EX --------------------
	//
	// Create a data buffer and initialize the values to provide some initial input to the ssbo
	int gpuData[10];
	const size_t GPU_DATASIZE = sizeof(gpuData) / sizeof(gpuData[0]);
	for (size_t i=0;i<GPU_DATASIZE;i++) {
		gpuData[i] = i;
	}

	// Load the buffer into an SSBO
//	SSBO ssbo;
	//ssbo.load(gpuData, sizeof(gpuData));

	// ----------------- VARIABLE SETUP -----------------
	//
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto tmpTime = lastTime;
	float frametime = 0;

	// Camera start position
	//window.getCam().setPos(glm::vec3(0, 0, -20));
	//window.getCam().setAngle(glm::vec3(0, -90, 0));

	// Shader uniforms
	shdrs.view.use();
	shdrs.view.setInt("GPU_DATASIZE", GPU_DATASIZE);

	while (!window.shouldClose()) {
		window.handleNoclip();

		window.getSize(&width, &height);
		window.getCam().updateProjectionView(width, height, 2000.0);

		//std::cout << window.getCam().getPos().x << " - " << window.getCam().getPos().y << " - " << window.getCam().getPos().z << std::endl;

		// Determine frame time for shader
		tmpTime = std::chrono::high_resolution_clock::now();
		frametime = std::chrono::duration<float>(tmpTime - lastTime).count();
		lastTime = tmpTime;

		shdrs.view.setFloat("frame_time", frametime);
		shdrs.post.setVec2("canvas_size", glm::vec2(width, height));
		// done frame time

		//ssbo.bind(0);

		// STANDARD PROCESSING
		// Setup drawing to the framebuffer
		fbo.bind();
		window.clear();
		{
			cube.draw(window, &shdrs.view);
		}

		// POST PROCESSING
		// Now draw the canvas to the main render buffer
	 	FBO::unbind();
		window.clear(GL_DEPTH_BUFFER_BIT);
		{
			canvas.draw(window, &shdrs.post);
		}

		//SSBO::unbind();

		// Now display to the screen
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
