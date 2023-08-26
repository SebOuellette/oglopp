#include "../Headers/shape.hpp"
#include <cstdlib>

Shape::Shape() {
	float tmpVerts[] = {
		 0.5, -0.5,  0.0,	1.0, 0.0, 0.0, // Bottom left
		-0.5, -0.5,  0.0,	0.0, 1.0, 0.0, // Bottom right
		 0.0,  0.5,  0.0,	0.0, 0.0, 1.0  // Top
	};

	this->vertices = new float[sizeof(tmpVerts) / sizeof(float)];
	std::memcpy(this->vertices, tmpVerts, sizeof(tmpVerts) / sizeof(float));

	// Create an empty vertex buffer object
	glGenBuffers(1, &this->VBO);
	// Bind the newly created buffer to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// Copy the vertex array data into the buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

	
	// ----- Vertex Array Object -----
	glGenVertexArrays(1, &this->VAO);
	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(this->VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Set the colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	std::cout << "Generated Triangle" << std::endl;
}

Shape::~Shape() {
	delete this->vertices;
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