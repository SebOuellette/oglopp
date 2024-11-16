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
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f});

		// Back face
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f});

		// Left face
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f});

		// Right face
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f});

		// Bottom face
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f});

		// Top face
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f});

		this->updateVAO();
	}
}
