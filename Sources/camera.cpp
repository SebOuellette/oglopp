#include "../Headers/oglopp.h"

namespace oglopp {
	Camera::Camera(glm::vec3 pos) {
		this->setPos(pos);

		this->lookAt(pos);

		// Set target to default, done so nothing is corrupt by accident
		//this->setTarget();
	}

	glm::vec3 Camera::getPos() {
		return this->_pos;
	}

	glm::vec3 Camera::getTarget() {
		return this->_target;
	}

	glm::vec3 Camera::getBack() {
		return this->_backward;
	}

	glm::vec3 Camera::getRight() {
		return this->_right;
	}

	glm::vec3 Camera::getUp() {
		return glm::cross(this->_backward, this->_right);
	}

	Camera& Camera::lookAt(glm::vec3 target) {
		// GLM function to calculate matrix with up, right, and back directions.
		this->_view = glm::lookAt(this->_pos, target, HLGL_WORLD_UP);

		return *this;
	}

	// Set the coordinate this camera will begin facing
	Camera& Camera::setTarget(glm::vec3 newPos) {
		// Copy argument to target variable
		this->_target = newPos;

		// Now calculate the direction (tutorial says this is actually backward direction)
		this->_backward = glm::normalize(this->_pos - this->_target);

		// Now trigger an update to the right direction, so all directions are synchronized
		this->_updateRight();

		// Return reference to camera object
		return *this;
	}

	Camera& Camera::_updateRight() {
		// Specify up direction in word coordinates
		glm::vec3 up = HLGL_WORLD_UP;

		// Update Right direction
		this->_right = glm::normalize(glm::cross(up, this->_backward));

		return *this;
	}

	Camera& Camera::setPos(glm::vec3 newPos) {
		this->_pos = newPos;

		return *this;
	}
}
