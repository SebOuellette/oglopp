#include "oglopp/defines.h"
#include "oglopp/obj/vbo.h"

namespace oglopp {
	/**
	 * @brief Create an OpenGL Vertex Buffer Object.
	 */
	VBO::VBO() {
		this->generate();
	}

	/**
	 * @brie Destroy the Vertex Buffer Object
	 */
	VBO::~VBO() {
		this->destroy();
	}
	

	/**
	 * @brief Generate the buffer object. To be called by constructor
	 * @return A reference to this buffer object
	 */
	VBO& VBO::generate() {
		// Create an empty vertex buffer object
		glGenBuffers(1, &this->index);

		return *this;
	}

	/**
	 * @brief Destroy the buffer object. To be called by destructor
	 * @return A reference to this buffer object (ready for next)
	 */
	VBO& VBO::destroy() {
		// From man glDeleteBuffer
		//  - 'ignores 0's and names that do not correspond
		//     with named buffers'
		glDeleteBuffers(1, &this->index);

		return *this;
	}

	/**
	 * @brief Bind the buffer.
	 * @return A reference to this buffer object
	 */
	VBO& VBO::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->index);

		return *this;
	}
	
	/**
	 * @brief Unbind the VBO
	 */
	void VBO::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/**
	 * @brief Update the buffer object.
	 * @param[in] data	Optional pointer to a data structure.
	 *  			To be used as defined in the overload func
	 * @return A reference to this buffer object
	 */
	VBO& VBO::update(void* data) {
		if (this->data.empty()) return *this;

		this->bind();

		// Copy the vertex array data into the buffer
		glBufferData(GL_ARRAY_BUFFER, 
			this->data.size(), // full size 
			this->data.data(), // Buffer
			GL_STATIC_DRAW);

		// Erase the local memory
		this->data.clear();

		return *this;
	}
}
