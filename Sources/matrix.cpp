#include "../Headers/oglopp.h"
#include <glm/ext/matrix_transform.hpp>

#include <sstream>

namespace oglopp {

	std::ostream& operator<<(std::ostream& os, glm::mat4 const& obj) {
		return Matrix::printMatrix(os, obj, 4, 4);
	}

	std::ostream& operator<<(std::ostream& os, glm::vec4 const& obj) {
		return Matrix::printVector(os, obj, 4);
	}

	std::ostream& operator<<(std::ostream& os, glm::vec3 const& obj) {
		return Matrix::printVector(os, obj, 3);
	}


	/* @brief Print an N by M matrix
	* @param[in]	matrix	The matrix object, where elements are expected to be indexed using [column][row]
	* @param[in]	cols	The number of columns in the matrix
	* @param[in]	rows	The number of rows in the matrix
	* @return				A reference to a stringstream object
	*/
	template<class T>
	std::ostream& Matrix::printMatrix(std::ostream& os, T matrix, short int cols, short int rows) {
		std::ostringstream outStream;
		
		for (uint8_t r=0;r<rows;r++) {
			if (r == 0) {
				outStream << "/";
			} else if (r == rows - 1) {
				outStream << "\\";
			} else {
				outStream << "|";
			}

			for (uint8_t c=0;c<cols;c++) {
				outStream << matrix[c][r] << ((c == cols-1) ? "" : " ");
			}

			if (r == 0) {
				outStream << "\\";
			} else if (r == rows - 1) {
				outStream << "/";
			} else {
				outStream << "|";
			}

			outStream << std::endl;
		}

		os << outStream.str();
		return os;
	}

	/* @brief Print an N dimension vector
	* @param[in]	vector	The matrix object, where elements are expected to be indexed using [row]
	* @param[in]	rows	The number of rows in the matrix
	* @return				A reference to a stringstream object
	*/
	template<class T>
	std::ostream& Matrix::printVector(std::ostream& os, T matrix, short int rows) {
		std::ostringstream outStream;
		
		for (uint8_t r=0;r<rows;r++) {
			if (r == 0) {
				outStream << "/";
			} else if (r == rows - 1) {
				outStream << "\\";
			} else {
				outStream << "|";
			}

			outStream << matrix[r];

			if (r == 0) {
				outStream << "\\";
			} else if (r == rows - 1) {
				outStream << "/";
			} else {
				outStream << "|";
			}

			outStream << std::endl;
		}

		os << outStream.str();
		return os;
	}

	/* @brief Translate some coordinate by an offset
	* @param[in]	coordinate	The original input coordinate
	* @param[in]	offset		The offset to adjust coordinate by
	* @return					The translated vector. For optimization purposes, this simply adds coordinate and offset instead of a matrix multiplication
	*/
	glm::vec3 Matrix::translate(glm::vec3 const& coordinate, glm::vec3 const& offset) {
		glm::vec4 quaternion = glm::vec4(coordinate, 1.0); // Create a quaterion

		// Generation translation matrix
		glm::mat4 operationMat = glm::translate(glm::mat4(1.0f), offset);

		// Do translation
		return quaternion * operationMat;
	}

	/* @brief Rotate some coordinate around a provided origin
	* @param[in]	coordinate	The original input coordinate
	* @param[in]	theta		The angle to rotate by, in radians
	* @param[in]	axis		The axis of rotation
	* @param[in]	origin		The origin to rotate around. This is optional, vec3(0, 0, 0) by default
	* @return					The rotated vector coordinates
	*/
	glm::vec3 Matrix::rotate(glm::vec3 const& coordinate, float const& theta, glm::vec3 const& axis, glm::vec3 const& origin) {
		glm::vec4 quaternion = glm::vec4(coordinate, 1.0);

		// Create the rotation matrix
		glm::mat4 operationMat = glm::rotate(glm::mat4(1.0f), theta, axis);

		std::cout << operationMat << std::endl;

		// Do rotation
		return quaternion * operationMat;
	}

	/* @brief Rotate some coordinate around a provided origin
	* @param[in]	coordinate	The original input coordinate
	* @param[in]	scale		The scale to scale the coordinates by
	* @param[in]	origin		The origin to scale around. This is optional, vec3(0, 0, 0) by default
	* @return					The rotated vector coordinates
	*/
	glm::vec3 Matrix::scale(glm::vec3 const& coordinate, glm::vec3 const& scale, glm::vec3 const& origin) {
		glm::vec4 quaternion = glm::vec4(coordinate, 1.0);

		// Create the rotation matrix
		glm::mat4 operationMat = glm::scale(glm::mat4(1.0f), scale);

		// Do rotation
		return quaternion * operationMat;
	}
}