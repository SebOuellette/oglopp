#ifndef OGLOPP_EBO_H
#define OGLOPP_EBO_H

#include <vector>

#include "oglopp/obj/bufferobj.h"

namespace oglopp {
	class EBO : public BufferObj<EBO> {
	private:
		std::vector<uint32_t> indices;
		size_t numSets;

		/**
		 * @brief Generate the buffer object
		 * @return A reference to this buffer object
		 */
		virtual EBO& generate() override;
		
		/**
		 * @brief Destroy the buffer object
		 * @return A reference to this buffer object (ready for next)
		 */
		virtual EBO& destroy() override;
	

	public:
		/**
		 * @brief Create an OpenGL Entity Buffer Object.
		 */
		EBO();

		/**
		 * @brief Destroy the Entity Buffer Object
		 */
		~EBO();		

		/**
		 * @brief Index push termination case. Increment count
		 * @return	A reference to this EBO
		 */
		EBO& push() {
			this->count++;
			return *this;
		}
		
		/**
		 * @brief Push a set of indices in order (CW or CCW)
		 * @description Increments getCount by one (no matter how many args given)
		 * @param[in] firstIndex	The first index to push
		 * @param[in] indices...	A list of more indices, in order
		 * @return			A reference to this EBO
		 */
		template <typename... Indices>
		EBO& push(uint32_t firstIndex, Indices... indices) {
			this->indices.push_back(firstIndex);

			return this->push(indices...);
		}
		
		/**
		 * @brief Bind the buffer.
		 * @return A reference to this buffer object
		 */
		virtual EBO& bind() override;
		
		/**
		 * @brief Unbind the EBO
		 */
		static void unbind();
		
		/**
		 * @brief Update the buffer object.
		 * @param[in] data	Ignored.
		 * @return A reference to this buffer object
		 */
		virtual EBO& update(void* data = nullptr) override; 
	};
}

#endif
