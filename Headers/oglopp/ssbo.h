#ifndef OGLOPP_SSBO_H
#define OGLOPP_SSBO_H

#include "defines.h"

namespace oglopp {
	class SSBO {
		public:
			enum MapMethod : uint16_t {
				READ	= GL_READ_ONLY,
				WRITE	= GL_WRITE_ONLY,
				BOTH	= GL_READ_WRITE
			};

			/* @brief Create a new SSBO object. Default constructor
	 		*/
			SSBO() = default;
			~SSBO() = default;

			/* @brief Copy the data in a pointer into the ssbo to be sent to the GPU on dispatch
			 * @param[in] buffer	A pointer to some buffer
			 * @param[in] size		The number of bytes to be read from the buffer
			 * @return				A status code. 0 for success. -1 for failure.
		 	*/
			int8_t load(void* buffer, size_t size);

			/* @brief Update a portion of the ssbo data
			 * @param[in] offset	The offset in bytes from the start of the ssbo buffer
			 * @param[in] buffer	A pointer to some buffer
			 * @param[in] size		The number of bytes to be read from the buffer
			 * @return 				A status code. 0 for success. -1 if the buffer was nullptr. -2 if the update would attempt to write out of range
			*/
			int8_t update(size_t offset, void* buffer, size_t size);

			/* @brief Bind the SSBO to some binding number. Default is 0 if not specified
			 * @param[in] binding	The binding number for the SSBO
			 * @return				A reference to the SSBO 0
		 	*/
			SSBO& bind(int binding = 0);

			/* @brief Unbind the bound SSBO
		 	*/
			static void unbind();

			/* @brief Map the SSBO to a buffer
			 * @param[in] method	The method of mapping. READ, WRITE, or BOTH
			 * @return 				A pointer to the mapped buffer
		 	*/
			void* map(MapMethod method = READ);

			/* @brief Unmap the mapped buffer
			 * @return A reference to this SSBO object
		 	*/
			SSBO& unmap();

		private:
			GLuint ssbo;
			size_t bufferSize;
	};
}

#endif
