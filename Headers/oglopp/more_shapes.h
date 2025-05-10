#ifndef OGLOPP_MORE_SHAPES_H
#define OGLOPP_MORE_SHAPES_H

#include "shape.h"


#define MAP_TO_COORD2(vertIndex, unitSize) (((static_cast<double>(vertIndex) * static_cast<double>(unitSize)) * 2) - 1.0)

namespace oglopp {
	/** @brief 2D Rectangle object
	*/
	class Rectangle : public Shape {
	public:
		Rectangle();
	};

	/** @brief 2D Triangle object
	*/
	class Triangle : public Shape {
	public:
		Triangle();
	};

	/** @brief 3D Cube object
	*/
	class Cube : public Shape {
	public:
		Cube();
	};

	/** @brief 3D Sphere object
	*/
	class Sphere : public Shape {
	private:
		// Buffer for normalized vectors when creating the shape
		glm::vec3 normal;

	public:
		/** @brief Create a 3D Sphere object.
		 *  @param[in] X_VERTS	The X resolution of the sphere.
		 *  @param[in] Y_VERTS 	The Y resolution of the sphere.
		 *  @param[in] Z_VERTS	The Z resolution of the sphere.
		 */
		Sphere(uint16_t const& X_VERTS = 10, uint16_t const& Y_VERTS = 10, uint16_t const& Z_VERTS = 10);

		/** @brief Push a point to a sphere. Normalize the provided point, then set the normal to the normal as well. Used for generating smooth spheres.
		 *  @param[in] point	The point on the domain/outer cube which will be normalized.
		 *	@return A reference to this spehre object
	 	*/
		Sphere& pushNormalizedPoint(const glm::vec3& point);

		/** @brief Loop through all the faces that should be pushed.
		 *  @
	 	*/
		template <typename C>
		Sphere& doAxisLoop(const uint16_t& OUTER_VERTS, const uint16_t& INNER_VERTS, const C& CALLBACK) {
			const double OUTER_UNIT = 1.0 / static_cast<double>(OUTER_VERTS);
			const double INNER_UNIT = 1.0 / static_cast<double>(INNER_VERTS);
			double Ao, Bo, Ai, Bi;

			// X layer - Front and Back.
			for (int16_t layer = -1; layer < 2; layer+=2) {
				for (uint16_t x = 0; x < OUTER_VERTS; x++) {
					Ao = MAP_TO_COORD2((layer < 0) ? x : x + 1, OUTER_UNIT);
					Bo = MAP_TO_COORD2((layer >= 0) ? x : x + 1, OUTER_UNIT);

					for (uint16_t y = 0; y < INNER_VERTS; y++) {
						Ai = MAP_TO_COORD2(y, INNER_UNIT);
						Bi = MAP_TO_COORD2(y + 1, INNER_UNIT);

						CALLBACK(Ao, Bo, Ai, Bi, layer);
					}
				}
			}

			return *this;
		}
	};
}

#endif
