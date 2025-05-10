#include "oglopp/more_shapes.h"

namespace oglopp {
	Rectangle::Rectangle() {
		this->pushTriangle(0, 1, 2);
		this->pushTriangle(2, 3, 0);

		this->pushPoint({ 0.5,  0.5, 0.1}, {1.0, 0.0, 0.0}, {1.0, 1.0});
		this->pushPoint({ 0.5, -0.5, 0.1}, {0.0, 1.0, 0.0}, {1.0, 0.0});
		this->pushPoint({-0.5, -0.5, 0.1}, {0.0, 0.0, 1.0}, {0.0, 0.0});
		this->pushPoint({-0.5,  0.5, 0.1}, {1.0, 0.0, 0.0}, {0.0, 1.0});

		// ..:: Initialization code ::..
		this->updateVAO();
	}

	Triangle::Triangle() {
		this->pushTriangle(0, 1, 2);

		this->pushPoint({-0.5, -1.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0});
		this->pushPoint({ 0.0,  0.0, 0.0}, {0.0, 1.0, 0.0}, {0.5, 1.0});
		this->pushPoint({ 0.5, -1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0});

		// ..:: Initialization code ::..
		this->updateVAO();
	}

	Cube::Cube() {
		//				Vector x, y, z			Normal x, y, z		Tex Coord x, y
		// Front face
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f});

		// Back face
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f});

		// Right? face
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f});

		// Left? face
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f});

		// Bottom face
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f});

		// Top face
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f});

		this->updateVAO();
	}

	/** @brief Create a 3D Sphere object.
	 *  @param[in] X_VERTS	The X resolution of the sphere.
	 *  @param[in] Y_VERTS 	The Y resolution of the sphere.
	 *  @param[in] Z_VERTS	The Z resolution of the sphere.
	 */
	Sphere::Sphere(uint16_t const& X_VERTS, uint16_t const& Y_VERTS, uint16_t const& Z_VERTS) {
		// Draw points for the Z axis (Front and Back)
		auto drawZAxis = [this](double& Ao, double& Bo, double& Ai, double& Bi, int16_t z) -> void {
			this->pushNormalizedPoint({Ai, Ao, z});
			this->pushNormalizedPoint({Ai, Bo, z});
			this->pushNormalizedPoint({Bi, Bo, z});

			this->pushNormalizedPoint({Bi, Bo, z});
			this->pushNormalizedPoint({Bi, Ao, z});
			this->pushNormalizedPoint({Ai, Ao, z});
		};

		// Draw points for the X axis (Left and Right)
		auto drawXAxis = [this](double& Ao, double& Bo, double& Ai, double& Bi, int16_t x) -> void {
			this->pushNormalizedPoint({x, Ai, Bo});
			this->pushNormalizedPoint({x, Bi, Bo});
			this->pushNormalizedPoint({x, Bi, Ao});

			this->pushNormalizedPoint({x, Bi, Ao});
			this->pushNormalizedPoint({x, Ai, Ao});
			this->pushNormalizedPoint({x, Ai, Bo});
		};

		// Draw points for the Y axis (Top and Bottom)
		auto drawYAxis = [this](double& Ao, double& Bo, double& Ai, double& Bi, int16_t y) -> void {
			this->pushNormalizedPoint({Bo, y, Ai});
			this->pushNormalizedPoint({Bo, y, Bi});
			this->pushNormalizedPoint({Ao, y, Bi});

			this->pushNormalizedPoint({Ao, y, Bi});
			this->pushNormalizedPoint({Ao, y, Ai});
			this->pushNormalizedPoint({Bo, y, Ai});
		};

		// Loop through each component on each face to push each vertex in the right order.
		this->doAxisLoop(X_VERTS, Y_VERTS, drawZAxis);
		this->doAxisLoop(Z_VERTS, Y_VERTS, drawXAxis);
		this->doAxisLoop(X_VERTS, Z_VERTS, drawYAxis);

		this->updateVAO(true, false);
	}

	/** @brief Push a point to a sphere. Normalize the provided point, then set the normal to the normal as well. Used for generating smooth spheres.
	 *  @param[in] point	The point on the domain/outer cube which will be normalized.
	 *  @return A reference to this spehre object
	 */
	Sphere& Sphere::pushNormalizedPoint(const glm::vec3& point) {
		this->normal = glm::normalize(point);
		this->pushPoint(this->normal, this->normal);

		return *this;
	}
}
