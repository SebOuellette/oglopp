#ifndef OGLOPP_VBO_H
#define OGLOPP_VBO_H

#include <vector>

#include <iostream>
#include "oglopp/util.h"
#include "oglopp/obj/component.h"
#include "oglopp/obj/bufferobj.h"

namespace oglopp {
	class VBO : public BufferObj<VBO> {
	private:
		// Each item is a float
		// <[v_x][v_y][v_y] [n_x][n_y][n_z] [t_x][t_y]> = 1 stride/point,3 items
		std::vector<uint8_t> data;

		/**
		 * @brief Generate the buffer object
		 * @return A reference to this buffer object
		 */
		virtual VBO& generate() override;
		
		/**
		 * @brief Destroy the buffer object
		 * @return A reference to this buffer object (ready for next)
		 */
		virtual VBO& destroy() override;
	public:
		

		/**
		 * @brief Create an OpenGL Vertex Buffer Object.
		 */
		VBO();

		/**
		 * @brief Destroy the Vertex Buffer Object
		 */
		~VBO();

		/**
		 * @brief Tempalte recursion push method. Termination case.
		 * @return	A reference to this VBO
		 */
		VBO& push() {
			// Called once no matter how many args are given.
			// We can increment the point count by 1 here
			this->count++;

			return *this;
		}
		
		/** 
		 * @brief Push a list of components to the VBO
		 * @description Increments getCount by one (no matter how many args given)
		 * @param[in] first	The first item to push
		 * @param[in] c...	The variadic list of additional components
		 * @return 		A reference to this VBO object
		*/
		template <typename First, typename... Components>
		VBO& push(First const& first, Components... c) {
			const uint8_t* ptr = safepconst_cast<const uint8_t*>(&first);

			// Insert the item
			this->data.insert(
				this->data.end(), 
				ptr, 
				ptr + sizeof(first));

			// Recurse
			return this->push(c...);
		}

		/**
		 * @brief Bind the buffer.
		 * @return A reference to this buffer object
		 */
		virtual inline VBO& bind() override;
		
		/**
		 * @brief Unbind the VBO
		 */
		static void unbind();
		
		/**
		 * @brief Update the buffer object.
		 * @param[in] data	Optional pointer to a data structure.
		 *  			To be used as defined in the overload func
		 * @return A reference to this buffer object
		 */
		virtual VBO& update(void* data = nullptr) override; 
	};
}

#endif
