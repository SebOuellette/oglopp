#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glad/gl.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

class Shape {
protected:
	float* vertices;
	unsigned int VAO;
	unsigned int VBO;
public:
	Shape();
	~Shape();

	unsigned int getVAO();
	unsigned int getVBO();
	float* getVertices();

	virtual void draw();
};

#endif // SHAPE_HPP
