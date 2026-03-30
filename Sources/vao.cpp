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
		glGenBuffers(1, &this->index);

		return *this;
	}

	/**
	 * @brief Destroy the buffer object. To be called by destructor
	 * @return A reference to this buffer object (ready for next)
	 */
	VAO& VAO::destroy() {
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
	VAO& VAO::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->index);

		return *this;
	}

	/**
	 * @brief Unbind the VAO
	 */
	void VAO::unbind() {
		glBindVertexArray(0);
	}

	/**
	 * @brief Update the buffer object.
	 * @param[in] data	Optional pointer to a data structure.
	 *  			To be used as defined in the overload func
	 * @return A reference to this buffer object
	 */
	VAO& VAO::update(void* data) {
		const size_t STRIDE = this->vbo.getStrideBytes();
		const size_t INDICES = this->ebo.count();

		// Does this need to happen first?
		this->bind();

		// Update Vertex Buffer Object
		this->vbo.update();

		// Update Entity Buffer Object
		// Will not do anything if no indices were pushed
		this->ebo.update();
		

		size_t offset = 0;
		int index = 0;

		// 1. Push vertex location
		CVertex::push(offset, index, STRIDE);
		
		// 2. Push Normal location
		CNormal::push(offset, index, STRIDE);

		// 3. Push Tex coord location
		CTexCoord::push(offset, index, STRIDE);


		// Unbind the vertex arrays
		VBO::unbind();
		VAO::unbind();

		return *this;
	}
}
