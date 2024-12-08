#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "oglopp/defines.h"
#include "oglopp/camera.h"

namespace oglopp {
	Camera::Camera(glm::dvec3 pos, glm::dvec3 target) {
		this->_view = glm::dmat4(1.f);
		this->_projection = glm::dmat4(1.f);

		this->fov = HLGL_DEFAULT_FOV;
		this->setAngle(target);
		this->setTarget(target);
		this->setPos(pos);
		this->lookAt(pos);


		// Set target to default, done so nothing is corrupt by accident
		//this->setTarget();
	}

	glm::dvec3 Camera::getPos() {
		return this->_pos;
	}

	glm::dvec3 Camera::getTarget() {
		return this->_target;
	}

	glm::dvec3 Camera::getBack() {
		return this->_backward;
	}

	glm::dvec3 Camera::getRight() {
		return this->_right;
	}

	glm::dvec3 Camera::getUp() {
		return glm::cross(this->_backward, this->_right);
	}

	float Camera::getFov() {
		return this->fov;
	}

	Camera& Camera::setFov(float fov) {
		this->fov = fov;
		return *this;
	}

	glm::dvec3 const& Camera::getAngle() {
		return this->_angle;
	}

	glm::dmat4 const& Camera::getView() {

		return this->_view;
	}

	glm::dmat4 const& Camera::getProjection() {
		return this->_projection;
	}

	/* @brief Face a target vector
	 * @param[in] vector	The normalized vector to face.
	 * @return				A constant reference to the updated view
	*/
	glm::dmat4 const& Camera::face(glm::dvec3 vector) {
		this->_view = glm::lookAt(this->_pos, this->_pos + vector, HLGL_WORLD_UP);

		return this->_view;
	}

	glm::dmat4 const& Camera::lookAt(glm::dvec3 target) {
		// GLM function to calculate matrix with up, right, and back directions.
		this->_view = glm::lookAt(this->_pos, target, HLGL_WORLD_UP);

		return this->_view;
	}

	Camera& Camera::_updateRight() {
		// Update Right direction
		this->_right = glm::normalize(glm::cross(HLGL_WORLD_UP, this->_backward));

		return *this;
	}

	Camera& Camera::setPos(glm::dvec3 const& newPos) {
		this->_pos = newPos;

		return *this;
	}

	/* @brief Translate the camera to a new position
	 * @param[in] offset	The offset to translate by
	*/
	Camera& Camera::translate(glm::dvec3 const& offset) {
		this->setPos(this->getPos() + offset);

		return *this;
	}

	// Set the coordinate this camera will begin facing
	Camera& Camera::setTarget(glm::dvec3 const& newPos) {
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
	Camera& Camera::setAngle(glm::dvec3 const& newAngle) {
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
	 * @param[in] width				The width of the window
	 * @param[in] height			The height of the window
	 * @param[in] farPlane			The far render plane
	 * @param[in] projectionType	The projection type, PERSPECTIVE or ORTHO
	 * @return	A reference to this Camera object
	*/
	Camera& Camera::updateProjectionView(int const& width, int const& height, double farPlane, Projection projectionType) {
		// Update the view matrix
		this->face(-this->getBack());

		// Update the projection matrix
		if (projectionType == Projection::PERSPECTIVE) {
			this->_projection = glm::perspective<double>(glm::radians(this->getFov()), static_cast<double>(width) / static_cast<double>(height), HLGL_RENDER_NEAR, farPlane);
		} else {
			//double orthoWidth = static_cast<double>(width) / height;
			//double orthoHeight = static_cast<double>(height) / width;

			double orthoWidth = static_cast<double>(width) / 500.0;
			double orthoHeight = static_cast<double>(height) / 500.0;
			this->_projection = glm::ortho<double>(-orthoWidth / 2.0, orthoWidth / 2.0, -orthoHeight / 2.0, orthoHeight / 2.0, static_cast<double>(HLGL_RENDER_NEAR), static_cast<double>(farPlane));
		}

		return *this;
	}
}
