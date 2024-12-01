#include "oglopp/ssbo.h"

namespace oglopp {

	/* @brief Copy the data in a pointer into the ssbo to be sent to the GPU on dispatch
	 * @param[in] buffer	A pointer to some buffer
	 * @param[in] size		The number of bytes to be read from the buffer
	 * @return				A status code. 0 for success. -1 for failure.
 	*/
	int8_t SSBO::load(void* buffer, size_t size) {
		if (buffer == nullptr) {
			return -1;
		}

		// Update the size
		this->bufferSize = size;

		// Prepare ssbo
		glGenBuffers(1, &this->ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);

		// Copy buffer into ssbo
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffer, GL_DYNAMIC_DRAW);

		// Unbind
		SSBO::unbind();

		return 0;
	}

	/* @brief Update a portion of the ssbo data
	 * @param[in] offset	The offset in bytes from the start of the ssbo buffer
	 * @param[in] buffer	A pointer to some buffer
	 * @param[in] size		The number of bytes to be read from the buffer
	 * @return 				A status code. 0 for success. -1 if the buffer was nullptr. -2 if the update would attempt to write out of range
 	*/
	int8_t SSBO::update(size_t offset, void* buffer, size_t size) {
		if (buffer == nullptr) {
			return -1;
		}

		// Check if this update would write out of range
		if (offset + size > this->bufferSize) {
			return -2;
		}

		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, ssbo);
		//this->use();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);

		// Bind the ssbo
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, buffer);

		// Unbind
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return 0;
	}

	/* @brief Bind the SSBO to some binding number. Default is 0 if not specified
	 * @param[in] binding	The binding number for the SSBO
	 * @return				A reference to the SSBO 0
 	*/
	SSBO& SSBO::bind(int binding) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssbo);
		return *this;
	}

	/* @brief Unbind the bound SSBO
 	*/
	void SSBO::unbind() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	/* @brief Map the SSBO to a buffer
	 * @param[in] method	The method of mapping. READ, WRITE, or BOTH
	 * @return 				A pointer to the mapped buffer
 	*/
	void* SSBO::map(MapMethod method) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
		void* mapped = glMapBuffer(GL_SHADER_STORAGE_BUFFER, method);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return mapped;
	}

	/* @brief Unmap the mapped buffer
 	*/
	SSBO& SSBO::unmap() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return *this;
	}
}
