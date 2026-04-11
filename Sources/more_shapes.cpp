#include "oglopp/more_shapes.h"

namespace oglopp {
	Rectangle::Rectangle() {
		this->getEBO().push(0, 1, 2);
		this->getEBO().push(2, 3, 0);

		this->pushPoint(CV( 0.5,  0.5, 0.1), CN(1.0, 0.0, 0.0), CT(1.0, 1.0));
		this->pushPoint(CV( 0.5, -0.5, 0.1), CN(0.0, 1.0, 0.0), CT(1.0, 0.0));
		this->pushPoint(CV(-0.5, -0.5, 0.1), CN(0.0, 0.0, 1.0), CT(0.0, 0.0));
		this->pushPoint(CV(-0.5,  0.5, 0.1), CN(1.0, 0.0, 0.0), CT(0.0, 1.0));

		// ..:: Initialization code ::..
		this->update<CV, CN, CT>();
	}

	Triangle::Triangle() {
		this->getEBO().push(0, 1, 2);

		this->pushPoint(CV(-0.5, -1.0, 0.0), CN(1.0, 0.0, 0.0), CT(0.0, 0.0));
		this->pushPoint(CV( 0.0,  0.0, 0.0), CN(0.0, 1.0, 0.0), CT(0.5, 1.0));
		this->pushPoint(CV( 0.5, -1.0, 0.0), CN(0.0, 0.0, 1.0), CT(1.0, 0.0));

		// ..:: Initialization code ::..
		this->update<CV, CN, CT>();
	}

	Cube::Cube() {
		//				Vector x, y, z			Normal x, y, z		Tex Coord x, y
		// Front face
		this->pushPoint(CV(-0.5f, -0.5f, -0.5f), CN(0.0f, 0.0f, -1.0f), CT(1.0f, 0.0f));
		this->pushPoint(CV(-0.5f,  0.5f, -0.5f), CN(0.0f, 0.0f, -1.0f), CT(1.0f, 1.0f));
		this->pushPoint(CV( 0.5f,  0.5f, -0.5f), CN(0.0f, 0.0f, -1.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV( 0.5f,  0.5f, -0.5f), CN(0.0f, 0.0f, -1.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV( 0.5f, -0.5f, -0.5f), CN(0.0f, 0.0f, -1.0f), CT(0.0f, 0.0f));
		this->pushPoint(CV(-0.5f, -0.5f, -0.5f), CN(0.0f, 0.0f, -1.0f), CT(1.0f, 0.0f));

		// Back face
		this->pushPoint(CV(-0.5f, -0.5f,  0.5f), CN(0.0f, 0.0f, 1.0f), CT(0.0f, 0.0f));
		this->pushPoint(CV( 0.5f, -0.5f,  0.5f), CN(0.0f, 0.0f, 1.0f), CT(1.0f, 0.0f));
		this->pushPoint(CV( 0.5f,  0.5f,  0.5f), CN(0.0f, 0.0f, 1.0f), CT(1.0f, 1.0f));
		this->pushPoint(CV( 0.5f,  0.5f,  0.5f), CN(0.0f, 0.0f, 1.0f), CT(1.0f, 1.0f));
		this->pushPoint(CV(-0.5f,  0.5f,  0.5f), CN(0.0f, 0.0f, 1.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV(-0.5f, -0.5f,  0.5f), CN(0.0f, 0.0f, 1.0f), CT(0.0f, 0.0f));

		// Right? face
		this->pushPoint(CV(-0.5f, -0.5f,  0.5f), CN(-1.0f, 0.0f, 0.0f), CT(1.0f, 0.0f));
		this->pushPoint(CV(-0.5f,  0.5f,  0.5f), CN(-1.0f, 0.0f, 0.0f), CT(1.0f, 1.0f));
		this->pushPoint(CV(-0.5f,  0.5f, -0.5f), CN(-1.0f, 0.0f, 0.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV(-0.5f,  0.5f, -0.5f), CN(-1.0f, 0.0f, 0.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV(-0.5f, -0.5f, -0.5f), CN(-1.0f, 0.0f, 0.0f), CT(0.0f, 0.0f));
		this->pushPoint(CV(-0.5f, -0.5f,  0.5f), CN(-1.0f, 0.0f, 0.0f), CT(1.0f, 0.0f));

		// Left? face
		this->pushPoint(CV( 0.5f, -0.5f,  0.5f), CN(1.0f, 0.0f, 0.0f), CT(0.0f, 0.0f));
		this->pushPoint(CV( 0.5f, -0.5f, -0.5f), CN(1.0f, 0.0f, 0.0f), CT(1.0f, 0.0f));
		this->pushPoint(CV( 0.5f,  0.5f, -0.5f), CN(1.0f, 0.0f, 0.0f), CT(1.0f, 1.0f));
		this->pushPoint(CV( 0.5f,  0.5f, -0.5f), CN(1.0f, 0.0f, 0.0f), CT(1.0f, 1.0f));
		this->pushPoint(CV( 0.5f,  0.5f,  0.5f), CN(1.0f, 0.0f, 0.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV( 0.5f, -0.5f,  0.5f), CN(1.0f, 0.0f, 0.0f), CT(0.0f, 0.0f));

		// Bottom face
		this->pushPoint(CV(-0.5f, -0.5f, -0.5f), CN(0.0f, -1.0f, 0.0f), CT(1.0f, 1.0f));
		this->pushPoint(CV( 0.5f, -0.5f, -0.5f), CN(0.0f, -1.0f, 0.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV( 0.5f, -0.5f,  0.5f), CN(0.0f, -1.0f, 0.0f), CT(0.0f, 0.0f));
		this->pushPoint(CV( 0.5f, -0.5f,  0.5f), CN(0.0f, -1.0f, 0.0f), CT(0.0f, 0.0f));
		this->pushPoint(CV(-0.5f, -0.5f,  0.5f), CN(0.0f, -1.0f, 0.0f), CT(1.0f, 0.0f));
		this->pushPoint(CV(-0.5f, -0.5f, -0.5f), CN(0.0f, -1.0f, 0.0f), CT(1.0f, 1.0f));

		// Top face
		this->pushPoint(CV(-0.5f,  0.5f, -0.5f), CN(0.0f, 1.0f, 0.0f), CT(1.0f, 0.0f));
		this->pushPoint(CV(-0.5f,  0.5f,  0.5f), CN(0.0f, 1.0f, 0.0f), CT(1.0f, 1.0f));
		this->pushPoint(CV( 0.5f,  0.5f,  0.5f), CN(0.0f, 1.0f, 0.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV( 0.5f,  0.5f,  0.5f), CN(0.0f, 1.0f, 0.0f), CT(0.0f, 1.0f));
		this->pushPoint(CV( 0.5f,  0.5f, -0.5f), CN(0.0f, 1.0f, 0.0f), CT(0.0f, 0.0f));
		this->pushPoint(CV(-0.5f,  0.5f, -0.5f), CN(0.0f, 1.0f, 0.0f), CT(1.0f, 0.0f));

		this->update<CV, CN, CT>();
	}
#if 0
	Cube2::Cube2() {  //   l/r    u/d     f/b\
		// Push the data
		this->pushVertex({ 0.5f,  0.5f,  0.5f});	// right up back
		this->pushVertex({ 0.5f,  0.5f, -0.5f}); 	// right up front
		this->pushVertex({ 0.5f, -0.5f,  0.5f}); 	// right down back
		this->pushVertex({ 0.5f, -0.5f, -0.5f});	// right down front
		this->pushVertex({-0.5f,  0.5f,  0.5f});	// left up back
		this->pushVertex({-0.5f,  0.5f, -0.5f});	// left up front
		this->pushVertex({-0.5f, -0.5f,  0.5f});	// left down back
		this->pushVertex({-0.5f, -0.5f, -0.5f});	// left down front

		this->pushNormal({ 0.0f, 0.0f, 1.0f});		// back
		this->pushNormal({ 0.0f, 1.0f, 0.0f});		// up
		this->pushNormal({ 1.0f, 0.0f, 0.0f});		// right
		this->pushNormal({ 0.0f, 0.0f,-1.0f});		// front
		this->pushNormal({ 0.0f,-1.0f, 0.0f});		// down
		this->pushNormal({-1.0f, 0.0f, 0.0f});		// left

		this->pushTexCoord({0.0, 0.0});	// bottom left
		this->pushTexCoord({0.0, 1.0});	// top left
		this->pushTexCoord({1.0, 0.0});	// bottom right
		this->pushTexCoord({1.0, 1.0});	// top right

		// Push the indices
		this->pushTriangle();

		this->updateVAO();
	}
#endif
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

		this->update<CV, CN>();
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
