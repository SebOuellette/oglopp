// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include "oglopp.h"
#include <cstdint>
#include <cstring>
#include <iostream>


using namespace oglopp;

#define ELEMENTS 20000

// We aren't pushing the standard <vector>,<normal>,<texture coord> set to the opengl VBO, we're just using points.
// We can define our own custom components (where vector, normal, and texture coord are each one component)
//
// In this example, we just want to define a single component that stores a uint32 ID number. (The particle ID)
struct CID : public Component<CID> {
	static constexpr uint32_t parts = 1;
	static constexpr uint16_t type = DataType::UINT32;

	uint32_t id;

	CID(uint32_t newId) : id(newId) {}

};

int main() {

	// Setup some window options to make it invisible
	Window::Settings options;
	options.visible = true;
	options.doFaceCulling = false;
	options.modifyPointSize = true;

	// Create the window
	Window window;
	window.create(800, 800, "HoneyLib OpenGL - Compute Graphical", options);

	// Compute shaders are defined seperately from other shaders since they can be executed on their own
	Compute compute(// Compute
		"#version 460 core\n"\
		"layout (local_size_x = 1) in;\n"\
		"layout (std430, binding = 0) buffer SSBO {\n"\
			"vec3 data[];\n"\
		"};\n"\
		\
		"uniform float time;\n"\
		\
		"void main() {\n"\
			"uvec3 index = gl_WorkGroupID;\n"\
			\
			"data[index.x] = vec3(sin(6 * (float(index.x) / gl_NumWorkGroups.x + time)), float(index.x) / gl_NumWorkGroups.x * 2.0 - 1.0, 0.0);\n"\
		"}", 
		ShaderType::RAW
	);


	// Define the rest of the shaders
	Shader shader(
		// Vertex
		"#version 460 core\n"\
		"layout (location = 0) in uint index;\n"\
		"layout (std430, binding = 0) buffer SSBO {\n"\
			"vec3 data[];\n"\
		"};\n"\
		\
		"void main() {\n"\
			"gl_Position = vec4(data[index], 1.0);\n"\
		"}",

		//Geometry
		"#version 460 core\n"\
		"layout (points) in;\n"\
		"layout (points, max_vertices = 1) out;\n"\
		\
		"void main() {\n"\
			"gl_Position = gl_in[0].gl_Position;\n"\
			"gl_PointSize = 1.0;\n"\
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
		ShaderType::RAW // use ::FILE to load from a file instead. 
	);

	// Vertices will be drawn as points, not triangulated. No lines or faces will be drawn.
	shader.setDrawType(POINTS);


	//
	// BUILD CUSTOM SHAPE
	//
	// We can build a custom shape that's just a set of points, then move those points around and draw them with the GPU. 
	//  This will also allow us to instance other objects on these points within the geometry shader if we so choose. 
	//
	// To do this, we don't care about vertex, norm, and texcoord, since we can't change those on the fly quickly. 
	//  Instead, we use an SSBO in a compute shader to move them 
	//     Note: (unnecessary, we can just use the SSBO in the vertex shader the same way, 
	//     but compute shaders are cool and let us specify an exact number of tasks to run)
	//
	Shape verts;
	for (uint32_t id = 0; id < ELEMENTS; id++) {
		verts.pushPoint(CID(id));
	}
	verts.update<CID>();


	//
	// Create the SSBO 
	// 
	// We can initialize the SSBO with some data. Here we allocate the data we want, set it to 0, then let the GPU handle it. 
	glm::vec4* data = new glm::vec4[ELEMENTS];
	std::memset(data, 0, sizeof(glm::vec4) * ELEMENTS);

	SSBO ssbo;
	ssbo.load(data, sizeof(glm::vec4) * ELEMENTS);

	// Send the data to the compute shader
	compute.setSSBO(&ssbo);
	delete[] data;






	float time = 0;

	std::cout << "Moving " << ELEMENTS << " verticies per frame in compute shader" << std::endl;
	while (!window.shouldClose()) {
		time += 0.002;

		compute.use();
		compute.setFloat("time", time);


		// Attempt to run once - this will double testVar and place the product into coolVar
		compute.dispatch(ELEMENTS);

		window.clear();
		verts.draw(window, &shader);

		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
