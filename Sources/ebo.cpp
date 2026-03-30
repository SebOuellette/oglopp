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
	 * @brief Get the number of sets of indices pushed to the list
	 * @return The number of individual indices
	 */
	size_t EBO::count() {
		return this->numSets;
	}

	/**
	 * @brief Push a single VAO point index
	 * @param[in] index	Index to the point in the VAO
	 * @return		A reference to this EBO
	 */
	EBO& EBO::pushSingle(uint32_t index) {
		this->indices.push_back(index);

		this->numSets++;

		return *this;
	}
	
	/**
	 * @brief Create a line of VAO point indices
	 * @param[in] idxA	First point index
	 * @param[in] idxB	Second point index
	 * @return		A reference to this EBO
	 */
	EBO& EBO::pushPair(uint32_t idxA, uint32_t idxB) {
		this->indices.push_back(idxA);
		this->indices.push_back(idxB);

		this->numSets++;

		return *this;
	}
	
	/**
	 * @brief Create a triangle of VAO point indices 
	 * @param[in] idxA	First point index
	 * @param[in] idxB	Second point index
	 * @param[in] idxC	Third point index
	 * @return		A reference to this EBO
	 */
	EBO& EBO::pushTriangle(uint32_t idxA, uint32_t idxB, uint32_t idxC) {
		this->indices.push_back(idxA);
		this->indices.push_back(idxB);
		this->indices.push_back(idxC);

		this->numSets++;

		return *this;
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
	 * @brief Unbind the EBO
	 */
	void EBO::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
			this->indices.size() * sizeof(uint32_t), 
			// Buffer
			this->indices.data(), 
			GL_STATIC_DRAW);

		// Erase the local memory
		this->indices.clear();

		return *this;
	}
}
