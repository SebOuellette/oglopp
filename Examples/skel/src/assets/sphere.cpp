
#include "assets/sphere.h"
#include <iostream>

#define MAP_TO_COORD(vertIndex, maxVerts) (((static_cast<double>(vertIndex) / static_cast<double>(maxVerts)) * 2) - 1.0)
#define MAP_TO_COORD2(vertIndex, unitSize) (((static_cast<double>(vertIndex) * static_cast<double>(unitSize)) * 2) - 1.0)



Sphere::Sphere(uint16_t const& X_VERTS, uint16_t const& Y_VERTS, uint16_t const& Z_VERTS) {
	double Ax, Ay, Az;
	double Bx, By, Bz;

	glm::vec3 point;

	// Precalculate the unit size
	const double X_UNIT = 1.0 / static_cast<double>(X_VERTS);
	const double Y_UNIT = 1.0 / static_cast<double>(Y_VERTS);
	const double Z_UNIT = 1.0 / static_cast<double>(Z_VERTS);

	for (int16_t z = -1; z < 2; z+=2) {
		for (uint16_t x = 0; x < X_VERTS; x++) {
			Ax = MAP_TO_COORD2(x, X_UNIT);
			Bx = MAP_TO_COORD2(x + 1, X_UNIT);

			for (uint16_t y = 0; y < Y_VERTS; y++) {
				Ay = MAP_TO_COORD2(y, Y_UNIT);
				By = MAP_TO_COORD2(y + 1, Y_UNIT);

				point = {Ax, Ay, z};
				this->pushPoint(point, glm::normalize(point));
				point = {Ax, By, z};
				this->pushPoint(point, glm::normalize(point));
				point = {Bx, By, z};
				this->pushPoint(point, glm::normalize(point));

				point = {Bx, By, z};
				this->pushPoint(point, glm::normalize(point));
				point = {Bx, Ay, z};
				this->pushPoint(point, glm::normalize(point));
				point = {Ax, Ay, z};
				this->pushPoint(point, glm::normalize(point));
			}
		}
	}

	for (int16_t x = -1; x < 2; x+=2) {
		for (uint16_t z = 0; z < Z_VERTS; z++) {
			Az = MAP_TO_COORD2(z, Z_UNIT);
			Bz = MAP_TO_COORD2(z + 1, Z_UNIT);

			for (uint16_t y = 0; y < Y_VERTS; y++) {
				Ay = MAP_TO_COORD2(y, Y_UNIT);
				By = MAP_TO_COORD2(y + 1, Y_UNIT);

				point = {x, Ay, Az};
				this->pushPoint(point, glm::normalize(point));
				point = {x, By, Az};
				this->pushPoint(point, glm::normalize(point));
				point = {x, By, Bz};
				this->pushPoint(point, glm::normalize(point));
				point = {x, By, Bz};
				this->pushPoint(point, glm::normalize(point));
				point = {x, Ay, Bz};
				this->pushPoint(point, glm::normalize(point));
				point = {x, Ay, Az};
				this->pushPoint(point, glm::normalize(point));
			}
		}
	}

	for (int16_t y = -1; y < 2; y+=2) {
		for (uint16_t x = 0; x < X_VERTS; x++) {
			Ax = MAP_TO_COORD2(x, X_UNIT);
			Bx = MAP_TO_COORD2(x + 1, X_UNIT);

			for (uint16_t z = 0; z < Z_VERTS; z++) {
				Az = MAP_TO_COORD2(z, Z_UNIT);
				Bz = MAP_TO_COORD2(z + 1, Z_UNIT);

				point = {Ax, y, Az};
				this->pushPoint(point, glm::normalize(point));
				point = {Ax, y, Bz};
				this->pushPoint(point, glm::normalize(point));
				point = {Bx, y, Bz};
				this->pushPoint(point, glm::normalize(point));
				point = {Bx, y, Bz};
				this->pushPoint(point, glm::normalize(point));
				point = {Bx, y, Az};
				this->pushPoint(point, glm::normalize(point));
				point = {Ax, y, Az};
				this->pushPoint(point, glm::normalize(point));
			}
		}
	}

	this->updateVAO(true, false);
}
