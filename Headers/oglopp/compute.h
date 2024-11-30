#ifndef OGLOPP_COMPUTE_H
#define OGLOPP_COMPUTE_H

#include "oglopp/glad/gl.h"
#include "oglopp/shader.h"

/*
 - Work group Size is defined in c++
 - Local size is defined in glsl with layout()
*/

namespace oglopp {
	/* @brief Compute shader class
	*/
	class Compute : public Shader {
	public:
		enum MapMethod : uint16_t {
			READ	= GL_READ_ONLY,
			WRITE	= GL_WRITE_ONLY,
			BOTH	= GL_READ_WRITE
		};

		typedef GLint group_t;

		/* @brief Compute default constructor
		 * @param[in] computeShader		The compute shader path or file contents
		 * @param[in] type				The shader type.  File or raw.
	 	*/
		Compute(const char* computeShader, ShaderType type);

		/* @brief Copy the data in a pointer into the ssbo to be sent to the GPU on dispatch
		 * @param[in] buffer	A pointer to some buffer
		 * @param[in] size		The number of bytes to be read from the buffer
		 * @return				A status code. 0 for success. -1 for failure.
	 	*/
		int8_t prepare(void* buffer, size_t size);

		/* @brief Update a portion of the ssbo data
		 * @param[in] offset	The offset in bytes from the start of the ssbo buffer
		 * @param[in] buffer	A pointer to some buffer
		 * @param[in] size		The number of bytes to be read from the buffer
		 * @return 				A status code. 0 for success. -1 for failure
	 	*/
		int8_t update(size_t offset, void* buffer, size_t size);

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

		/* @brief Map the SSBO to a buffer
		 * @param[in] method	The method of mapping. READ, WRITE, or BOTH
		 * @return 				A pointer to the mapped buffer
	 	*/
		void* map(MapMethod method = READ);

		/* @brief Unmap the mapped buffer
	 	*/
		Compute& unmap();

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
		GLuint ssbo;

		size_t bufSize;

		/* @brief Load a list of shaders into this shader object
		 * @param[in] computeShader		The compute shader path or file contents
		 * @param[in] type				The shader type.  File or raw.
	 	*/
		void load(const char* computeShader, ShaderType type);
	};
}


#endif
