#include "../Headers/oglopp.h"

namespace oglopp {
	Rectangle::Rectangle() {
		this->pushTriangle(0, 1, 2);
		this->pushTriangle(2, 3, 0);

		this->pushPoint({ 0.5,  0.5, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0});
		this->pushPoint({ 0.5, -0.5, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0});
		this->pushPoint({-0.5, -0.5, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0});
		this->pushPoint({-0.5,  0.5, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0});

		// ..:: Initialization code ::..
		this->updateVAO();
	}
}
