#include "../Headers/rectangle.hpp"
#include <iostream>

Rectangle::Rectangle() {
	this->indices = new int[INDICES] {
		0, 1, 2, // first triangle
		2, 3, 0  // second triangle
	};

	this->vertices = new float[VERTICES * 2] {
		 0.5,  0.5, 0.0,		 1.0f, 0.0f, 0.0f,  // top right - red
		 0.5, -0.5, 0.0,		 0.0f, 1.0f, 0.0f,  // bottom right - green
		-0.5, -0.5, 0.0,		0.0f, 0.0f, 1.0f,  // bottom left - blue
		-0.5,  0.5, 0.0,		1.0f, 0.0f, 0.0f// top left - red
	};

	// ..:: Initialization code :: ..
	this->updateVAO();
}

Rectangle::~Rectangle() {
	//delete this->vertices;
	delete[] this->indices;
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	glDeleteVertexArrays(1, &this->VAO);
}

Rectangle& Rectangle::updateEBO() {
	// Create the element buffer object
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(int), this->indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_BINDING, this->EBO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	return *this;
}

Rectangle& Rectangle::updateVAO() {
	glGenVertexArrays(1, &this->VAO);

	// 1. bind Vertex Array Object
	glBindVertexArray(this->VAO);
	
	// Update Vertex Buffer Object
	this->updateVBO();

	// Update Entity Buffer Object
	this->updateEBO();
	
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
	glEnableVertexAttribArray(0);

	// Set the colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	

	// Bind the buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO); 

	// Unbind the vertex array
	glBindVertexArray(0);

	return *this;
}

Rectangle& Rectangle::updateVBO() {
	// Create an empty vertex buffer object
	glGenBuffers(1, &this->VBO);
	
	// Bind the newly created buffer to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	
	// Copy the vertex array data into the buffer 
	glBufferData(GL_ARRAY_BUFFER, VERTICES * 2 * sizeof(float), this->vertices, GL_STATIC_DRAW);

	return *this;
}

void Rectangle::draw() {
	// The very beginning
	// https://cdn.honeybeeks.net/rNoI7KT2nH.png

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}