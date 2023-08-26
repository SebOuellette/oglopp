#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"

class Rectangle : public Shape {
private:
	unsigned int EBO;
public:
	Rectangle();

	void draw();
};

#endif // SHAPE_HPP