#include "oglopp/defines.h"
#include "oglopp/obj/ebo.h"

namespace oglopp {
	/**
	 * @brief Create an OpenGL Vertex Buffer Object.
	 */
	EBO::EBO() {
		this->generate();
	}

	/**
	 * @brie Destroy the Vertex Buffer Object
	 */
	EBO::~EBO() {
		this->destroy();
	}
	
	/**
	 * @brief Get a reference to the index buffer
	 * @return A reference to the index buffer vertex
	 */
	std::vector<uint32_t>& EBO::getIndices() {
		return this->indices;
	}

	/**
	 * @brief Get the number of indices in the list
	 * @return The number of indices
	 */
	size_t EBO::count() {
		return this->indices.size();
	}

	/**
	 * @brief Generate the buffer object. To be called by constructor
	 * @return A reference to this buffer object
	 */
	EBO& EBO::generate() {
		// Create an empty vertex buffer object
		glGenBuffers(1, &this->index);

		return *this;
	}

	/**
	 * @brief Destroy the buffer object. To be called by destructor
	 * @return A reference to this buffer object (ready for next)
	 */
	EBO& EBO::destroy() {
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
	EBO& EBO::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index);

		return *this;
	}


	/**
	 * @brief Update the buffer object.
	 * @param[in] data	Ignored.
	 * @return A reference to this buffer object
	 */
	EBO& EBO::update(void* data) {
		// Exit early if there are no indices
		if (this->count() == 0) return *this;

		this->bind();	

		// Copy the vertex array data into the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			// Stride
			this->indices.size() * HLGL_EBO_COMPONENTS * sizeof(uint32_t), 
			// Buffer
			this->indices.data(), 
			GL_STATIC_DRAW);

		// Erase the local memory
		this->indices.clear();

		return *this;
	}
}
