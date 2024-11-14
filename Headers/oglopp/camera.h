#ifndef OGLOPP_CAMERA_H
#define OGLOPP_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace oglopp {
	/* @brief Camera object
	*/
	class Camera {
	private:
		// Positions
		glm::vec3 _pos;
		glm::vec3 _target;
		glm::vec3 _angle;

		// Directions
		glm::vec3 _backward; // The tutorial says this is the backward direction
		glm::vec3 _right;

		// Camera stuff
		glm::mat4 _view;

		float fov;

		Camera& _updateRight();

	public:
		Camera(glm::vec3 pos = glm::vec3(0, 0.0, 0.0));

		// Positions
		glm::vec3 getPos();
		glm::vec3 getTarget();
		// Native directions
		glm::vec3 getBack();
		glm::vec3 getRight();
		glm::vec3 getUp();
		float getFov();

		Camera& setFov(float);

		glm::vec3 const& getAngle();

		glm::mat4 const& getView();

		/* @brief Face a target vector
		 * @param[in] vector	The normalized vector to face.
		 * @return				A constant reference to the updated view
		*/
		glm::mat4 const& face(glm::vec3 vector) ;

		// Alternative for all the crap above?
		glm::mat4 const& lookAt(glm::vec3 target);

		Camera& setPos(glm::vec3 const& newPos = glm::vec3(0.0f, 0.0f, 3.0f));
		Camera& translate(glm::vec3 const& offset);
		Camera& setTarget(glm::vec3 const& newPos = glm::vec3(0.0f, 0.0f, 0.0f));
		Camera& setAngle(glm::vec3 const& newAngle = glm::vec3(0, 0, 0));

		/* @brief Aim the camera by a pitch/yaw
		 * @param[in] pitch	The pitch in degrees to rotate by
		 * @param[in] yaw	The yaw in degrees to rotate by
		 * @return 			A reference to this Camera object
	 	*/
		Camera& aimBy(float pitch, float yaw);
	};
}

#endif
