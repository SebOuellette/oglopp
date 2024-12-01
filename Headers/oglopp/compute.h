#ifndef OGLOPP_COMPUTE_H
#define OGLOPP_COMPUTE_H

#include "oglopp/glad/gl.h"
#include "oglopp/shader.h"
#include "oglopp/ssbo.h"

/*
 - Work group Size is defined in c++
 - Local size is defined in glsl with layout()
*/

namespace oglopp {
	/* @brief Compute shader class
	*/
	class Compute : public Shader {
	public:
		typedef GLint group_t;

		/* @brief Compute default constructor
		 * @param[in] computeShader		The compute shader path or file contents
		 * @param[in] type				The shader type.  File or raw.
		 * @param[in] newBinding		The new binding int for the SSBO
	 	*/
		Compute(const char* computeShader, ShaderType type, GLuint newBinding = 0);

		/* @brief Set the SSBO object used by the compute shader.
		 * @param[in] newSSBO	A constant reference to the new SSBO object
		 * @return				A reference to this compute object
	 	*/
		Compute& setSSBO(SSBO* newSSBO);

		/* @brief Get a reference to the loaded SSBO object
		 * @return	A reference to the ssbo object
	 	*/
		SSBO* getSSBO();

		/* @brief Dispatch some groups with the loaded compute shader
		 * @param[in] xGroups	The number of groups in the x dimension (not optional)
		 * @param[in] yGroups	The number of groups in the y dimension (optional - default is 1)
		 * @param[in] zGroups	The number of groups in the z dimension (optional - default is 1)
		 * @return				A status code. 0 for success. -1 for failure.
	 	*/
		int8_t dispatch(group_t xGroups, group_t yGroups = 1, group_t zGroups = 1);

		/* @brief Dispatch a buffer object specifying number of groups with the loaded compute shader
		 * @param[in] pBufferObject	A GLintptr pointing to some buffer object
		 * @return					A status code. 0 for success. -1 for failure.
	 	*/
		int8_t dispatch(GLintptr pBufferObject);

		/* @brief Get a constant reference to the SSBO binding
		 * @return The SSBO binding int
	 	*/
		const GLuint& getBinding();

		/* @brief Check if a count of groups is valid.
		 * @param[in] xCount	The count of groups to check in the X
		 * @param[in] yCount	The count of groups to check in the Y
		 * @param[in] zCount	The count of groups to check in the Z
		 * @return				True if valid, false otherwise
		*/
		static const bool groupCountIsValid(group_t xCount, group_t yCount, group_t zCount);

		/* @brief Check if a group's size is valid.
		 * @param[in] groupSize		The size of a group to check
		 * @return					True if valid, false otherwise
	 	*/
		static const bool groupSizeIsValid(group_t groupSize);

	private:
		//GLuint ssbo;
		const GLuint binding;

		//size_t bufSize;
		SSBO* ssbo;

		/* @brief Load a list of shaders into this shader object
		 * @param[in] computeShader		The compute shader path or file contents
		 * @param[in] type				The shader type.  File or raw.
	 	*/
		void load(const char* computeShader, ShaderType type);
	};
}


#endif
