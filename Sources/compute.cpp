#include "oglopp/compute.h"
#include "oglopp/glad/gl.h"

#include <iostream>

namespace oglopp {
	/* @brief Compute default constructor
	 * @param[in] computeShader		The compute shader path or file contents
	 * @param[in] type				The shader type.  File or raw.
	 * @param[in] newBinding		The new binding point for the SSBO
 	*/
	Compute::Compute(const char* computeShader, ShaderType type, GLuint newBinding) : binding(newBinding), ssbo(nullptr) {
		this->load(computeShader, type);
	}

	/* @brief Set the SSBO object used by the compute shader.
	 * @param[in] newSSBO	A constant reference to the new SSBO object
	 * @return				A reference to this compute object
 	*/
	Compute& Compute::setSSBO(SSBO* newSSBO) {
		this->ssbo = newSSBO;

		return *this;
	}

	/* @brief Get a reference to the loaded SSBO object
	 * @return	A reference to the ssbo object
 	*/
	SSBO* Compute::getSSBO() {
		return this->ssbo;
	}

	/* @brief Dispatch some groups with the loaded compute shader
	 * @param[in] xGroups	The number of groups in the x dimension (not optional)
	 * @param[in] yGroups	The number of groups in the y dimension (optional - default is 1)
	 * @param[in] zGroups	The number of groups in the z dimension (optional - default is 1)
	 * @return				A status code. 0 for success. -1 for failure.
 	*/
	int8_t Compute::dispatch(group_t xGroups, group_t yGroups, group_t zGroups) {
		if (this->ssbo == nullptr || !Compute::groupCountIsValid(xGroups, yGroups, zGroups)) {
			return -1;
		}

		// Use this shader program
		this->ssbo->bind(this->binding);
		this->use();

		// Dispatch
		glDispatchCompute(xGroups, yGroups, zGroups);

		// Ensure all shader writes are visible
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Unbind
		SSBO::unbind();

		return 0;
	}

	/* @brief Dispatch a buffer object specifying number of groups with the loaded compute shader
	 * @param[in] pBufferObject	A GLintptr pointing to some buffer object
	 * @return					A status code. 0 for success. -1 for failure.
 	*/
	int8_t Compute::dispatch(GLintptr pBufferObject) {
		if (this->ssbo == nullptr)
			return -1;

		// Use this shader program
		this->ssbo->bind();
		this->use();

		// Dispatch
		glDispatchComputeIndirect(pBufferObject);

		// Ensure all shader writes are visible
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Unbind
		SSBO::unbind();

		return 0;
	}

	/* @brief Get a constant reference to the SSBO binding
	 * @return The SSBO binding int
 	*/
	const GLuint& Compute::getBinding() {
		return this->binding;
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
