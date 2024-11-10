#include "../Headers/oglopp.h"

namespace oglopp {
	Triangle::Triangle() {
		this->pushTriangle(0, 1, 2);

		this->pushPoint({-0.5, -1.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0});
		this->pushPoint({ 0.0,  0.0, 0.0}, {0.0, 1.0, 0.0}, {0.5, 1.0});
		this->pushPoint({ 0.5, -1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0});

		// ..:: Initialization code ::..
		this->updateVAO();
	}
}
