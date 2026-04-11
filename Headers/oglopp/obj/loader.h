#ifndef OGLOPP_MATRIX_H
#define OGLOPP_MATRIX_H

#include <vector>
#include <string>

#include "oglopp/obj/component.h"
#include "oglopp/obj/vao.h"

namespace oglopp {
	// https://en.wikipedia.org/wiki/Wavefront_.obj_file :)
	class Loader {
	private:
		// List of shape components for OBJ construction 
		std::vector<CV> cVertices;
		std::vector<CT> cTexCoords;
		std::vector<CN> cNormals;

		/**
		 * @brief Push a set of indices. If negative, create a new 0 element.
	 	 * @param[in] vbo	The VBO to push vertices to
		 * @param[in] vertIndex	Vertex index
		 * @param[in] texIndex	Texture index
		 * @param[in] normIndex	Normal index
		 * @return 		A reference to this Loader object
		 */
		Loader& pushFace(VBO& vbo, int64_t vertIndex, int64_t texIndex, int64_t normIndex);

		/**
		 * @brief Push a triangle of indices to the EBO
		 * @param[in] ebo	The EBO to push indices to
		 * @param[in] start	The start index of the triangle fan
		 * @param[in] offset	The number of points so far in this frame
		 */
		Loader& pushTriangleIndices(EBO& ebo, uint32_t start, uint32_t offset);

		/**
		 * @brief Push a triangle of indices to the EBO
		 * @param[in] ebo	The EBO to push indices to
		 * @param[in] start	The start index of the triangle fan
		 * @param[in] 
		 */
		Loader& pushTriangleIndices(EBO& ebo, uint32_t start, uint32_t offsetA, uint32_t offsetB);

	public:
		/**
		 * @brief Build a new OBJ loader object
		 */
		Loader();

		/**
		 * @brief Construct a new shape from an obj file
		 * @param[in] filename	The path of the obj file
		 * @param[in] vao	The VAO object to build
		 * @return A reference to this objloader object
		 */
		Loader& construct(std::string const& filename, VAO& vao);	
	};
}

#endif
