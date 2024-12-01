// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include "oglopp.h"
#include "oglopp/shader.h"
#include "oglopp/shape.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <glm/ext/scalar_uint_sized.hpp>
#include <iostream>
#include <cmath>


using namespace oglopp;

#define ELEMENTS 4

int main() {

	// Setup some window options to make it invisible
	Window::Settings options;
	options.visible = true;
	options.doFaceCulling = false;
	options.modifyPointSize = true;

	// Create the window
	Window window;
	window.create(800, 800, "HoneyLib OpenGL - Compute Graphical", options);

	// // Initialize our shader object
	Compute compute(// Compute
	"#version 460 core\n"\
	"layout (local_size_x = 1) in;\n"\
	"layout (std430, binding = 0) buffer SSBO {\n"\
		"vec4 data[];\n"\
	"};\n"\
	\
	"uniform double time;\n"\
	\
	"void main() {\n"\
		"uvec3 index = gl_WorkGroupID;\n"\
		\
		"data[index.x] = vec4(data[index.x].x + 1.0, 1.0, float(index.x) / float(gl_NumWorkGroups.x), 0.5);\n"\
	"}\n", ShaderType::RAW);

	Shader shader(
		// Vertex
		"#version 460 core\n"\
		"layout (location = 0) in uint index;\n"\
		\
		"void main() {\n"\
			"gl_Position = vec4(0.1 * index, 0.1 * index, 0.0, 1.0);\n"\
		"}",

		//Geometry
		"#version 460 core\n"\
		"layout (points) in;\n"\
		"layout (points, max_vertices = 1) out;\n"\
		\
		"void main() {\n"\
			"gl_Position = gl_in[0].gl_Position;\n"\
			"gl_PointSize = 10.0;\n"\
			"EmitVertex();\n"\
			"EndPrimitive();\n"\
		"}",

		// Fragment
		"#version 460 core\n"\
		\
		"in vec3 FragPos;\n"\
		"out vec4 FragColor;\n"\
		\
		"void main() {\n"\
			"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"\
		"}",
		ShaderType::RAW
	);

	shader.setDrawType(POINTS);

	// Create a list of vertices with just an ID. the position will be provided in an SSBO calculated by a compute shader
	Shape verts;
	for (uint32_t id = 0; id < ELEMENTS; id++) {
		verts.pushValue(&id, sizeof(id));
		verts.incrementVerts();
	}
	verts.finalizePoints(Shape::UINT32);


	glm::vec4* data = new glm::vec4[ELEMENTS];
	std::memset(data, 0, sizeof(glm::vec4) * ELEMENTS);

	// Send the data to the compute shader
	compute.prepare(data, sizeof(glm::vec4) * ELEMENTS);
	delete[] data;

	while (!window.shouldClose()) {

	#if 0
		// Attempt to run once - this will double testVar and place the product into coolVar
		compute.dispatch(ELEMENTS);

		// Map the SSBO and read the result (By default, map() will use READONLY mode)
		glm::vec4* result = static_cast<glm::vec4*>(compute.map());
		if (result == nullptr) {
			std::cerr << "Uhh.. the map pointer was null? That can happen?" << std::endl;
			return -1;
		}

		for (int i=0;i<ELEMENTS;i++) {
			std::cout << "Element [" << i << "] - (" << result[i].r << ", " << result[i].g << ", " << result[i].b << ", " << result[i].a << ")" << std::endl;
		}

		compute.unmap();
	#endif

		window.clear();

		//compute.bindSSBO();
		verts.draw(window, &shader);
		//compute.unbindSSBO();

		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
