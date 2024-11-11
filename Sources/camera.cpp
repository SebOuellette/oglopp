#include "../Headers/oglopp.h"
#include <glm/ext/quaternion_geometric.hpp>

namespace oglopp {
	Camera::Camera(glm::vec3 pos) {
		this->_view = glm::mat4(1.f);

		this->setAngle(glm::vec3(0.0));
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

	glm::vec3 const& Camera::getAngle() {
		return this->_angle;
	}

	glm::mat4 const& Camera::getView() {

		return this->_view;
	}

	glm::mat4 const& Camera::lookAt(glm::vec3 target) {
		// GLM function to calculate matrix with up, right, and back directions.
		this->_view = glm::lookAt(this->_pos, target, HLGL_WORLD_UP);

		return this->_view;
	}

	Camera& Camera::_updateRight() {
		// Specify up direction in word coordinates
		glm::vec3 up = HLGL_WORLD_UP;

		// Update Right direction
		this->_right = glm::normalize(glm::cross(up, this->_backward));

		return *this;
	}

	Camera& Camera::setPos(glm::vec3 const& newPos) {
		this->_pos = newPos;

		return *this;
	}

	/* @brief Translate the camera to a new position
	 * @param[in] offset	The offset to translate by
	*/
	Camera& Camera::translate(glm::vec3 const& offset) {
		this->setPos(this->getPos() + offset);

		return *this;
	}

	// Set the coordinate this camera will begin facing
	Camera& Camera::setTarget(glm::vec3 const& newPos) {
		// Copy argument to target variable
		this->_target = newPos;

		this->lookAt(newPos);

		// Now calculate the direction (tutorial says this is actually backward direction)
		this->_backward = glm::normalize(this->_pos - this->_target);

		// Now trigger an update to the right direction, so all directions are synchronized
		this->_updateRight();

		// Return reference to camera object
		return *this;
	}

	/* @brief Set the angle of the camera
	 * @param[in] newAngle	The new angle to set
	 * @return				A reference to this camera
	*/
	Camera& Camera::setAngle(glm::vec3 const& newAngle) {
		this->_angle = newAngle;

		glm::vec3 direction;
		direction.x = cos(glm::radians(newAngle.y)) * cos(glm::radians(newAngle.x));
		direction.y = sin(glm::radians(newAngle.x));
		direction.z = sin(glm::radians(newAngle.y)) * cos(glm::radians(newAngle.x));

		// Update the new target
		this->_backward = glm::normalize(direction);
		this->_target = this->getPos() - this->_backward;

		this->lookAt(this->_target);

		this->_updateRight();
		
		return *this;
	}

}
