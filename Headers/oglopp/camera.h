#ifndef OGLOPP_CAMERA_H
#define OGLOPP_CAMERA_H

#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3_precision.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "defines.h"

namespace oglopp {
	/** @brief Camera object
	*/
	class Camera {
	private:
		// Positions
		glm::dvec3 _pos;
		glm::dvec3 _target;
		glm::dvec3 _angle;

		// Directions
		glm::dvec3 _backward; // The tutorial says this is the backward direction
		glm::dvec3 _right;

		// Camera stuff
		glm::dmat4 _view;
		glm::dmat4 _projection;

		float fov;

		Camera& _updateRight();

	public:
		enum Projection {
			ORTHO,
			PERSPECTIVE,
		};

		Camera(glm::dvec3 pos = glm::vec3(0, 0.0, 0.0), glm::dvec3 target = glm::vec3(0.0, 0.0, 1.0));

		// Positions
		glm::dvec3 getPos();
		glm::dvec3 getTarget();
		// Native directions
		glm::dvec3 getBack();
		glm::dvec3 getRight();
		glm::dvec3 getUp();
		float getFov();

		Camera& setFov(float);

		glm::dvec3 const& getAngle();

		glm::dmat4 const& getView();
		glm::dmat4 const& getProjection();


		/** @brief Face a target vector
		 * @param[in] vector	The normalized vector to face.
		 * @return				A constant reference to the updated view
		*/
		glm::dmat4 const& face(glm::dvec3 vector) ;

		// Alternative for all the crap above?
		glm::dmat4 const& lookAt(glm::dvec3 target);

		Camera& setPos(glm::dvec3 const& newPos = glm::dvec3(0.0f, 0.0f, 3.0f));
		Camera& translate(glm::dvec3 const& offset);
		Camera& setTarget(glm::dvec3 const& newPos = glm::dvec3(0.0f, 0.0f, 0.0f));
		Camera& setAngle(glm::dvec3 const& newAngle = glm::dvec3(0, 0, 0));

		/** @brief Aim the camera by a pitch/yaw
		 * @param[in] pitch	The pitch in degrees to rotate by
		 * @param[in] yaw	The yaw in degrees to rotate by
		 * @return 			A reference to this Camera object
	 	*/
		Camera& aimBy(float pitch, float yaw);

		/** @brief Update the projection and view matrices to be referenced by each object
		 * @param[in] width				The width of the window
		 * @param[in] height			The height of the window
		 * @param[in] farPlane			The far render plane
		 * @param[in] projectionType	The projection type, PERSPECTIVE or ORTHO
		 * @return	A reference to this Camera object
		*/
		Camera& updateProjectionView(int const& width, int const& height, double farPlane = HLGL_RENDER_FAR, Projection projectionType = PERSPECTIVE);
	};
}

#endif
