#include "../Headers/shape.hpp"
#include <cstdlib>

#define VERTS 18
#define VERT_SIZE VERTS * sizeof(float)

Shape::Shape() {
	// float tmpVerts[] = {
	// 	 0.5, -0.5,  0.0,	1.0, 0.0, 0.0, // Bottom left
	// 	-0.5, -0.5,  0.0,	0.0, 1.0, 0.0, // Bottom right
	// 	 0.0,  0.5,  0.0,	0.0, 0.0, 1.0  // Top
	// };

	this->vertices = new float[VERTS] {
		 0.5, -0.5,  0.0,	1.0, 0.0, 0.0, // Bottom left
		-0.5, -0.5,  0.0,	0.0, 1.0, 0.0, // Bottom right
		 0.0,  0.5,  0.0,	0.0, 0.0, 1.0  // Top
	};
	
	//std::memcpy(this->vertices, tmpVerts, VERT_SIZE);

	// Vertex buffer obejct
	

	// ----- Vertex Array Object -----
	updateVAO();
}

Shape::~Shape() {
	delete this->vertices;
}

Shape& Shape::updateVAO() {
	glGenVertexArrays(1, &this->VAO);
	// Initialization code (done once (unless your object frequently changes))
	
	// 1. bind Vertex Array Object
	glBindVertexArray(this->VAO);

	this->updateVBO();
	
	// 2. copy our vertices array in a buffer for OpenGL to use
	//glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	//glBufferData(GL_ARRAY_BUFFER, VERT_SIZE, this->vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Reset the bound buffer since we set the data and are done
	
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Set the colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return *this;
}

Shape& Shape::updateVBO() {
	// Create an empty vertex buffer object
	glGenBuffers(1, &this->VBO);
	
	// Bind the newly created buffer to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	
	// Copy the vertex array data into the buffer 
	glBufferData(GL_ARRAY_BUFFER, VERT_SIZE, this->vertices, GL_STATIC_DRAW);

	return *this;
}

unsigned int Shape::getVAO() {
	return this->VAO;
}

unsigned int Shape::getVBO() {
	return this->VBO;
}

float* Shape::getVertices() {
	return this->vertices;
}

void Shape::draw() {
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}