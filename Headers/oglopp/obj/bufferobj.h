#ifndef OGLOPP_BUFFEROBJ_H
#define OGLOPP_BUFFEROBJ_H

#include <cstdint>

namespace oglopp {
	template <typename Child>
	class BufferObj {
	protected:
		uint32_t index;
		uint32_t count;
		
		/**
		 * @brief Generate the buffer object
		 * @return A reference to this buffer object
		 */
		virtual BufferObj& generate() = 0;

		/**
		 * @brief Destroy the buffer object
		 * @return A reference to this buffer object (ready for next)
		 */
		virtual BufferObj& destroy() = 0;
	

	public:
		/*
		 * @brief OpenGL Buffer Object Wrapper. Not meant to be called by users of OGLOPP. 
		 */
		inline BufferObj() : index(0), count(0) {}

		/**
		 * @brief Get the buffer index 
		 * @return The opengl buffer index
		 */
		inline uint32_t getIndex() const {
			return this->index;
		}	

		/**
		 * @brief Get the number of items pushed. Up to the overloaded method.
		 * @return The number of items pushed
		 */
		inline uint32_t getCount() const {
			return this->count;
		}

		/**
		 * @brief Bind the buffer.
		 * @return A reference to this buffer object
		 */
		virtual inline BufferObj& bind() = 0;

		/**
		 * @brief Unbind the buffer (static virtual)
		 */
		static inline void unbind() {
			Child::unbind();	
		}

		/**
		 * @brief Update the buffer object.
		 * @param[in] data	Optional pointer to a data structure.
		 *  			To be used as defined in the overload func
		 * @return A reference to this buffer object
		 */
		virtual BufferObj& update(void* data = nullptr) = 0;
	};
}

#endif
