// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include "oglopp.h"
#include <cstdlib>
#include <iostream>
#include <cmath>


using namespace oglopp;

struct DataStruct {
	float testVar;
	float coolVar;
};

#define ELEMENTS 200000

int main() {

	// Setup some window options to make it invisible
	Window::Settings options;
	options.visible = false;

	// Create the window
	Window window;
	window.create(800, 800, "HoneyLib OpenGL - Compute", options);

	// // Initialize our shader object
	Compute compute(// Compute
	"#version 460 core\n"\
	"layout (local_size_x = 1, local_size_y = 1) in;\n"\
	"struct DataStruct {\n"\
		"float testVar;\n"\
		"float coolVar;\n"\
	"};\n"\
	\
	"layout (std430, binding = 0) buffer SSBO {\n"\
		"DataStruct data[];\n"\
	"};\n"\
	\
	"void main() {\n"\
		"uvec3 index = gl_GlobalInvocationID;\n"\
		\
		"data[index.x].coolVar = data[index.x].testVar * 2.0;\n"\
	"}\n", ShaderType::RAW);

	DataStruct* data = new DataStruct[ELEMENTS];

	// Initialize testVar with random values
	for (int i=0;i<ELEMENTS;i++) {
		data[i].testVar = static_cast<float>(static_cast<double>(rand()) / RAND_MAX);
	}

	// Create a new SSBO object from the data (We could also do this first, then map the buffer, then fill it, if we wanted. That would remove the need for the 'data' array.. but this is a demonstration so it's fine.)
	SSBO ssbo;
	ssbo.load(data, sizeof(DataStruct) * ELEMENTS);
	delete[] data;

	// Tell the compute shader to use the new SSBO object
	compute.setSSBO(&ssbo);

	// Send the data to the compute shader
	//compute.prepare(data, sizeof(DataStruct) * ELEMENTS);

	// Attempt to run once - this will double testVar and place the product into coolVar
	compute.dispatch(ELEMENTS);

	// Map the SSBO and read the result (By default, map() will use READONLY mode)
	DataStruct* result = static_cast<DataStruct*>(ssbo.map());
	if (result == nullptr) {
		std::cerr << "Uhh.. the map pointer was null? That can happen?" << std::endl;
		return -1;
	}

	for (int i=0;i<ELEMENTS;i++) {
		std::cout << "Element [" << i << "] - " << result[i].testVar << " * 2 = " << result[i].coolVar << std::endl;
	}

	ssbo.unmap();

	return 0;
}
