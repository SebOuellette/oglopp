#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"

#define INDICES 6
#define VERTICES 4 * 3
#define STRIDE 6 * sizeof(float)

class Rectangle : public Shape {
private:
	unsigned int EBO;
	int* indices;
public:
	Rectangle();
	~Rectangle();

	Rectangle& updateEBO();
	Rectangle& updateVAO();
	Rectangle& updateVBO();
	void draw();
};

#endif // SHAPE_HPP