#ifndef OGLOPP_PROJECTION_H
#define OGLOPP_PROJECTION_H

#include <oglopp/window.h>
#include <oglopp/shader.h>

namespace oglopp {
	/**
	 * @brief Handle the scale, angle, and position and the model, view, projection matrices
	 */
	class Projection {
	private:
		glm::dvec3 size;
		glm::dvec3 angle;
		glm::dvec3 position;

	protected:	
		/** 
		 * @brief Update uniform MVP. To be called by draw()
		 * @description Updates 'model', 'view', 'projection', and 'rotation' uniforms
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	A pointer to the shader object
		 * @return		A reference to this shape object
		 */
		Projection& updateUniformMVP(Window& window, Shader* pShader);

	public:
		/**
		 * @brief Create a new projection based object. Must be inherited.
		 * @description Handles position, rotation, and scale
		 */
		Projection();

		//
		// Basic Getters
		//

		/** @brief Get the position of this projection
		 * @return The position of this projection
		*/
		glm::dvec3 const& getPosition();

		/** @brief Get the angle of this projection
		 * @return The angle of this projection
		*/
		glm::dvec3 const& getAngle();
		
		/** @brief Get the scale factor
		 * @return The scaling factor
		*/
		glm::dvec3 const& getScale();


		//
		// Basic Setters
		//

		/** @brief Set the position of this projection in world space
		 * @param[in] newPosition	The position in world space
		 * @return			A reference to this position
		*/
		Projection& setPosition(glm::dvec3 newPosition);

		/** @brief Set the angle of this projection in radians for each axis
		 * @param[in] newAngle		The angle to set to
		 * @return 			A reference to this projection object
		*/
		Projection& setAngle(glm::dvec3 newAngle);

		/** @brief Set the scale of the projection
		 * @param[in] newScale	The new scale for this projection
		 * @return 		A reference to this projection object
		*/
		Projection& setScale(glm::dvec3 newScale);

		
		//
		// Transformations
		//

		/** @brief Translate this projection some coordinates in world space
		 * @param[in] offset	The offset to translate by
		 * @return		A reference to this projection object
		*/
		Projection& translate(glm::dvec3 offset);

		/** @brief Rotate this projection around its local origin
		 * @param[in] offset	The offset to rotate by in radians for each axis
		 * @return		A reference to this projection object
		*/
		Projection& rotate(glm::dvec3 offset);

		/** @brief Apply a scaling factor to the projection
		 * @param[in] offset	The new scale for this projection
		 * @return		A reference to this projection object
		*/
		Projection& scale(glm::dvec3 offset);

		/**
		 * @brief Projections are meant to be drawn, but can't exist alone
		 * 		Instances of projections will define how to draw
		 */
		virtual Projection& draw(Window& window, Shader* pShader = nullptr) = 0;
	};
}

#endif
