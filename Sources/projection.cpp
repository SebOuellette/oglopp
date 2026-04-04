#include "oglopp/obj/projection.h"

#include <glm/gtc/matrix_transform.hpp>

namespace oglopp {
	/**
	 * @brief Create a new projection based object. Must be inherited.
	 * @description Handles position, rotation, and scale
	 */
	Projection::Projection(): size(1), angle(0), position(0) {}

	/** 
	 * @brief Update uniform MVP. To be called by draw()
	 * @description Updates 'model', 'view', 'projection', and 'rotation' uniforms
	 * @param[in] window	A reference to the window object
	 * @param[in] pShader	A pointer to the shader object
	 * @return		A reference to this shape object
	 */
	Projection& Projection::updateUniformMVP(Window& window, Shader* pShader) {
		if (pShader == nullptr) return *this;

		// ..:: Model Matrix ::..
		glm::dmat4 model(1.f); // Accumulate changes
		glm::dmat4 rotation(1.f); // Used for transforming normals
		
		// This is kinda backwards but it gives the correct result
		// projection = model * position * rotation * scale)
		model *= glm::translate(model, this->position); 
		if (this->getAngle() != glm::dvec3(0)) {
			rotation = glm::rotate<double>(rotation, this->getAngle().x, glm::dvec3(1.0, 0.0, 0.0f));
			rotation = glm::rotate<double>(rotation, this->getAngle().y, glm::dvec3(0.0, 1.0, 0.0f));
			rotation = glm::rotate<double>(rotation, this->getAngle().z, glm::dvec3(0.0, 0.0, 1.0f));
			model *= rotation;
		}

		model = glm::scale(model, this->getScale());

		// ..:: Apply Elements ::..
		pShader->use();
		pShader->setMat4("model", model);
		pShader->setMat4("view", window.getCam().getView());
		pShader->setMat4("projection", window.getCam().getProjection());
		pShader->setMat4("rotation", rotation);

		return *this;
	}


	/** @brief Get the position of this shape
	* @return The position of this shape
	*/
	glm::dvec3 const& Projection::getPosition() {
		return this->position;
	}

	/** @brief Get the angle of this shape
	* @return The angle of this shape
	*/
	glm::dvec3 const& Projection::getAngle() {
		return this->angle;
	}
	
	/** @brief Get the scale factor
	* @return The scaling factor
	*/
	glm::dvec3 const& Projection::getScale() {
		return this->size;
	}


	/** @brief Set the position of this shape in world space
	* @param[in] newPosition	The position in world space
	* @return					A reference to this position
	*/
	Projection& Projection::setPosition(glm::dvec3 newPosition) {
		this->position = newPosition;
		return *this;
	}

	/** @brief Set the angle of this shape in radians for each axis
	* @param[in] newAngle		The angle to set to
	* @return 					A reference to this shape object
	*/
	Projection& Projection::setAngle(glm::dvec3 newAngle) {
		this->angle = newAngle;
		return *this;
	}
	
	/** @brief Set the scale of the shape
	* @param[in] newScale	The new scale for this shape
	* @return 				A reference to this shape object
	*/
	Projection& Projection::setScale(glm::dvec3 newScale) {
		this->size = newScale;

		return *this;
	}
	
	
	/** @brief Translate this shape some coordinates in world space
	* @param[in] offset	The offset to translate by
	* @return				A reference to this shape object
	*/
	Projection& Projection::translate(glm::dvec3 offset) {
		this->position += offset;
		return *this;
	}

	/** @brief Rotate this shape around its local origin
	* @param[in] offset	The offset to rotate by in radians for each axis
	* @return				A reference to this shape object
	*/
	Projection& Projection::rotate(glm::dvec3 offset) {
		this->angle += offset;
		return *this;
	}

	/** @brief Apply a scaling factor to the shape
	* @param[in] offset	The new scale for this shape
	* @return				A reference to this shape object
	*/
	Projection& Projection::scale(glm::dvec3 offset) {
		this->size *= offset;

		return *this;
	}
}

