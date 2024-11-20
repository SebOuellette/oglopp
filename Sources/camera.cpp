#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "oglopp/defines.h"
#include "oglopp/camera.h"

namespace oglopp {
	Camera::Camera(glm::vec3 pos) {
		this->_view = glm::mat4(1.f);
		this->_projection = glm::mat4(1.f);

		this->fov = HLGL_DEFAULT_FOV;
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

	float Camera::getFov() {
		return this->fov;
	}

	Camera& Camera::setFov(float fov) {
		this->fov = fov;
		return *this;
	}

	glm::vec3 const& Camera::getAngle() {
		return this->_angle;
	}

	glm::mat4 const& Camera::getView() {

		return this->_view;
	}

	glm::mat4 const& Camera::getProjection() {
		return this->_projection;
	}

	/* @brief Face a target vector
	 * @param[in] vector	The normalized vector to face.
	 * @return				A constant reference to the updated view
	*/
	glm::mat4 const& Camera::face(glm::vec3 vector) {
		this->_view = glm::lookAt(this->_pos, this->_pos + vector, HLGL_WORLD_UP);

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

		this->face(-this->_backward);

		this->_updateRight();

		return *this;
	}

	/* @brief Aim the camera by a pitch/yaw
	 * @param[in] pitch	The pitch in degrees to rotate by
	 * @param[in] yaw	The yaw in degrees to rotate by
	 * @return 			A reference to this Camera object
 	*/
	Camera& Camera::aimBy(float pitch, float yaw) {
		glm::vec3 angleCopy = this->_angle;

		yaw *= 0.04;
		pitch *= 0.04;

		angleCopy.y += yaw;
		angleCopy.x += pitch;

		if (angleCopy.x >= 89.0)
			angleCopy.x = 89.0;
		else if (angleCopy.x <= -89.0)
			angleCopy.x = -89.0;

		return this->setAngle(angleCopy);
	}

	/* @brief Update the projection and view matrices to be referenced by each object
	 * @brief width		The width of the window
	 * @brief height	The height of the window
	 * @return	A reference to this Camera object
 	*/
	Camera& Camera::updateProjectionView(int const& width, int const& height) {
		// Update the view matrix
		this->face(-this->getBack());

		// Update the projection matrix
		this->_projection = glm::perspective<float>(glm::radians(this->getFov()), static_cast<float>(width) / static_cast<float>(height), HLGL_RENDER_NEAR, HLGL_RENDER_FAR);

		return *this;
	}
}
