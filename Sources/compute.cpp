#include "oglopp/compute.h"
#include "oglopp/glad/gl.h"

#include <iostream>

namespace oglopp {
	/* @brief Compute default constructor
	 * @param[in] computeShader		The compute shader path or file contents
	 * @param[in] type				The shader type.  File or raw.
	 * @param[in] newBinding		The new binding point for the SSBO
 	*/
	Compute::Compute(const char* computeShader, ShaderType type, GLuint newBinding) : binding(newBinding) {
		this->load(computeShader, type);
	}

	/* @brief Copy the data in a pointer into the ssbo to be sent to the GPU on dispatch
	 * @param[in] buffer	A pointer to some buffer
	 * @param[in] size		The number of bytes to be read from the buffer
	 * @return				A status code. 0 for success. -1 for failure.
 	*/
	int8_t Compute::prepare(void* buffer, size_t size) {
		if (buffer == nullptr) {
			return -1;
		}

		// Update the size
		this->bufSize = size;

		// Prepare ssbo
		glGenBuffers(1, &this->ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);

		// Copy buffer into ssbo
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffer, GL_DYNAMIC_DRAW);

		// Free up the buffer or apply or something idk
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, ssbo);

		// Unbind
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return 0;
	}

	/* @brief Update a portion of the ssbo data
	 * @param[in] offset	The offset in bytes from the start of the ssbo buffer
	 * @param[in] buffer	A pointer to some buffer
	 * @param[in] size		The number of bytes to be read from the buffer
	 * @return 				A status code. 0 for success. -1 if the buffer was nullptr. -2 if the update would attempt to write out of range
 	*/
	int8_t Compute::update(size_t offset, void* buffer, size_t size) {
		if (buffer == nullptr) {
			return -1;
		}

		// Check if this update would write out of range
		if (offset + size > this->bufSize) {
			return -2;
		}

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, ssbo);
		this->use();

		// Bind the ssbo
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, buffer);

		// Unbind
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return 0;
	}

	/* @brief Dispatch some groups with the loaded compute shader
	 * @param[in] xGroups	The number of groups in the x dimension (not optional)
	 * @param[in] yGroups	The number of groups in the y dimension (optional - default is 1)
	 * @param[in] zGroups	The number of groups in the z dimension (optional - default is 1)
	 * @return				A status code. 0 for success. -1 for failure.
 	*/
	int8_t Compute::dispatch(group_t xGroups, group_t yGroups, group_t zGroups) {
		if (!Compute::groupCountIsValid(xGroups, yGroups, zGroups)) {
			return -1;
		}

		// Use this shader program
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, ssbo);
		this->use();


		// Dispatch
		glDispatchCompute(xGroups, yGroups, zGroups);

		// Ensure all shader writes are visible
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Unbind
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return 0;
	}

	/* @brief Dispatch a buffer object specifying number of groups with the loaded compute shader
	 * @param[in] pBufferObject	A GLintptr pointing to some buffer object
	 * @return					A status code. 0 for success. -1 for failure.
 	*/
	int8_t Compute::dispatch(GLintptr pBufferObject) {
		// Use this shader program
		this->use();

		// Dispatch
		glDispatchComputeIndirect(pBufferObject);

		// Ensure all shader writes are visible
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		return 0;
	}

	/* @brief Map the SSBO to a buffer
	 * @param[in] method	The method of mapping. READ, WRITE, or BOTH
	 * @return 				A pointer to the mapped buffer
 	*/
	void* Compute::map(MapMethod method) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
		void* mapped = glMapBuffer(GL_SHADER_STORAGE_BUFFER, method);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return mapped;
	}

	/* @brief Unmap the mapped buffer
 	*/
	Compute& Compute::unmap() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		return *this;
	}

	/* @brief Get a constant reference to the SSBO binding
	 * @return The SSBO binding int
 	*/
	const GLuint& Compute::getBinding() {
		return this->binding;
	}

	Compute& Compute::bindSSBO() {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, ssbo);
		return *this;
	}

	void Compute::unbindSSBO() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	/* @brief Check if a count of groups is valid.
	 * @param[in] xCount	The count of groups to check in the X
	 * @param[in] yCount	The count of groups to check in the Y
	 * @param[in] zCount	The count of groups to check in the Z
	 * @return				True if valid, false otherwise
 	*/
	const bool Compute::groupCountIsValid(group_t xCount, group_t yCount, group_t zCount) {
		if (xCount < 1 || yCount < 1 || zCount < 1) {
			return false;
		}

		GLint maxGroupsX, maxGroupsY, maxGroupsZ;
		maxGroupsX = maxGroupsY = maxGroupsZ = 16;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &maxGroupsX);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &maxGroupsY);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &maxGroupsZ);

		return !(xCount > maxGroupsX || yCount > maxGroupsY || zCount > maxGroupsZ);
	}

	/* @brief Check if a group's size is valid.
	 * @param[in] groupSize		The size of a group to check
	 * @return					True if valid, false otherwise
 	*/
	const bool Compute::groupSizeIsValid(group_t groupSize) {
		return true;
	}

	/* @brief Load a list of shaders into this shader object
	 * @param[in] computeShader		The compute shader path or file contents
	 * @param[in] type				The shader type.  File or raw.
 	*/
	void Compute::load(const char* computeShader, ShaderType type) {
		if (computeShader == nullptr)
			return;

		// Compile the shader
		uint32_t computeIndex = Shader::loadShaderFile(computeShader, type, ShaderStep::COMPUTE);

		// Create compute program
		this->ID = glCreateProgram();

		// Attach the shader to the new program
		glAttachShader(this->ID, computeIndex);

		// Link the program
		glLinkProgram(this->ID);

		// print linking errors if any
		GLint success;
		char infoLog[512];
		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "[Oglopp] Compute shader linking failed!\n" << infoLog << std::endl;
		}

		// Delete the shader now that it's been linked
		glDeleteShader(computeIndex);
	}
}
