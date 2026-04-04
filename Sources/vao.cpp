#include "oglopp/defines.h"
#include "oglopp/obj/vao.h"

namespace oglopp {
	/**
	 * @brief Create an OpenGL Vertex Buffer Object.
	 */
	VAO::VAO() {
		this->generate();
	}

	/**
	 * @brie Destroy the Vertex Buffer Object
	 */
	VAO::~VAO() {
		this->destroy();
	}
	
	/**
	 * @brief Get a reference to the VBO object
	 * @return A reference to the VBO object
	 */
	VBO& VAO::getVBO() {
		return this->vbo;
	}
	
	/**
	 * @brief Get a reference to the EBO object
	 * @return A reference to the EBO object
	 */
	EBO& VAO::getEBO() {
		return this->ebo;
	}

	/**
	 * @brief Generate the buffer object. To be called by constructor
	 * @return A reference to this buffer object
	 */
	VAO& VAO::generate() {
		// Create an empty vertex buffer object
		glGenVertexArrays(1, &this->index);

		return *this;
	}

	/**
	 * @brief Destroy the buffer object. To be called by destructor
	 * @return A reference to this buffer object (ready for next)
	 */
	VAO& VAO::destroy() {
		// From man glDeleteVertexArrays
		//  - 'ignores 0's and names that do not correspond
		//     with named buffers'
		glDeleteVertexArrays(1, &this->index);

		return *this;
	}
	
	/**
	 * @brief Bind the buffer.
	 * @return A reference to this buffer object
	 */
	VAO& VAO::bind() {
		glBindVertexArray(this->index);

		return *this;
	}

	/**
	 * @brief Unbind the VAO
	 */
	void VAO::unbind() {
		glBindVertexArray(0);
	}
}
