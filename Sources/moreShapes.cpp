#include "../Headers/oglopp.h"

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
		// this->pushTriangle(0, 1, 2);
		// this->pushTriangle(2, 0, 3);
		// this->pushTriangle(3, 4, 5);
		// this->pushTriangle(5, 3, 2);
		// this->pushTriangle(2, 5, 6);
		// this->pushTriangle(6, 2, 1);
		// this->pushTriangle(1, 6, 7);
		// this->pushTriangle(8, 1, 0);
		// this->pushTriangle(0, 3, 7);
		// this->pushTriangle(7, 3, 4);
		// this->pushTriangle(4, 5, 6);
		// this->pushTriangle(6, 4, 7);

		// // // https://cdn2.honeybeeks.net/ff1c38e8cf2.png
		// this->pushPoint({ 0.5, -0.5,  0.5}, {1.0, 0.0, 0.0}, {1.0, 0.0});
		// this->pushPoint({ 0.5,  0.5,  0.5}, {0.0, 1.0, 0.0}, {1.0, 1.0});
		// this->pushPoint({ 0.5,  0.5, -0.5}, {0.0, 0.0, 1.0}, {0.0, 1.0});
		// this->pushPoint({ 0.5, -0.5, -0.5}, {0.0, 1.0, 0.0}, {0.0, 0.0});
		// this->pushPoint({-0.5, -0.5, -0.5}, {1.0, 0.0, 0.0}, {0.0, 0.0});
		// this->pushPoint({-0.5,  0.5, -0.5}, {0.0, 1.0, 0.0}, {0.0, 1.0});
		// this->pushPoint({-0.5,  0.5,  0.5}, {0.0, 0.0, 1.0}, {0.0, 0.0});
		// this->pushPoint({-0.5, -0.5,  0.5}, {0.0, 1.0, 0.0}, {0.0, 0.0});
		//
		//

#if 0

		// When you load it like this, the shader has to be written specifically to handle objects without the vertex color attribute.. which is fine.. but yea idk what's best. probably this. Who cares about vertex color?

		// Need to convert this array to the push format as defined above. We can exclude the middle vector of 3, so we just have this->pushPoint({x, y, z}, {a, b});
		// Front face
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f});

		// Back face
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f});

		// Left face
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f});

		// Right face
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f});

		// Bottom face
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f});

		// Top face
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f});

		this->updateVAO(false, true);
#else
		// Front face
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f});

		// Back face
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {1.0f, 0.5f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 0.0f}, {0.0f, 0.0f});

		// Left face
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f});

		// Right face
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f});

		// Bottom face
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f});

		// Top face
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.5f, 0.0f, 1.0f}, {0.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f, -0.5f}, {0.5f, 0.0f, 1.0f}, {1.0f, 1.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.5f, 0.0f, 1.0f}, {1.0f, 0.0f});
		this->pushPoint({ 0.5f,  0.5f,  0.5f}, {0.5f, 0.0f, 1.0f}, {1.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f,  0.5f}, {0.5f, 0.0f, 1.0f}, {0.0f, 0.0f});
		this->pushPoint({-0.5f,  0.5f, -0.5f}, {0.5f, 0.0f, 1.0f}, {0.0f, 1.0f});

		this->updateVAO();
#endif


		
	}
}
