#ifndef OGLOPP_MATRIX_H
#define OGLOPP_MATRIX_H

#include <ostream>
#include <glm/vec3.hpp>

namespace oglopp {
/** @brief Utilities for matrix math. Translate, scale, and rotate coordinates.
	*/
	class Matrix {
	public:
		/** @brief Print an N by M matrix
		 * @param[in]	matrix	The matrix object, where elements are expected to be indexed using [column][row]
		 * @param[in]	cols	The number of columns in the matrix
		 * @param[in]	rows	The number of rows in the matrix
		 * @return				A reference to a stringstream object
		*/
		template<class T>
		static std::ostream& printMatrix(std::ostream& os, T matrix, short int cols, short int rows);

		/** @brief Print an N dimension vector
		 * @param[in]	vector	The matrix object, where elements are expected to be indexed using [row]
		 * @param[in]	rows	The number of rows in the matrix
		 * @return				A reference to a stringstream object
		*/
		template<class T>
		static std::ostream& printVector(std::ostream& os, T matrix, short int rows);

		/** @brief Translate some coordinate by an offset
		 * @param[in]	coordinate	The original input coordinate
		 * @param[in]	offset		The offset to adjust coordinate by
		 * @return					The translated vector. For optimization purposes, this simply adds coordinate and offset instead of a matrix multiplication
		*/
		static glm::vec3 translate(glm::vec3 const& coordinate, glm::vec3 const& offset);

		/** @brief Rotate some coordinate around a provided origin
		 * @param[in]	coordinate	The original input coordinate
		 * @param[in]	theta		The angle to rotate by, in radians
		 * @param[in]	axis		The axis of rotation
		 * @param[in]	origin		The origin to rotate around. This is optional, vec3(0, 0, 0) by default
		 * @return					The rotated vector coordinates
		*/
		static glm::vec3 rotate(glm::vec3 const& coordinate, float const& theta, glm::vec3 const& axis, glm::vec3 const& origin = glm::vec3(0));

		/** @brief Rotate some coordinate around a provided origin
		 * @param[in]	coordinate	The original input coordinate
		 * @param[in]	scale		The scale to scale the coordinates by
		 * @param[in]	origin		The origin to scale around. This is optional, vec3(0, 0, 0) by default
		 * @return					The rotated vector coordinates
		*/
		static glm::vec3 scale(glm::vec3 const& coordinate, glm::vec3 const& scale, glm::vec3 const& origin = glm::vec3(0));
	};
}

#endif
