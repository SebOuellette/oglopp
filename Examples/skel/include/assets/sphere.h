#ifndef SPHERE_H
#define SPHERE_H

#include <oglopp/shape.h>

class Sphere : public oglopp::Shape {
public:
	Sphere(uint16_t const& X_VERTS = 10, uint16_t const& Y_VERTS = 10, uint16_t const& Z_VERTS = 10);
};

#endif
