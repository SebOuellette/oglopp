#ifndef OGLOPP_VAO_H
#define OGLOPP_VAO_H

#include "oglopp/obj/vbo.h"
#include "oglopp/obj/ebo.h"

namespace oglopp {
	template <typename... Components>
	struct VAOUpdater;

	template<>
	struct VAOUpdater<> {
		static void strideSum(size_t& stride, uint32_t& count) {
			stride = 0;
			count = 0;
		}

		static void update(size_t const STRIDE, size_t offset = 0, int index = 0){}
	};

	template <typename First, typename... Components>
	struct VAOUpdater<First, Components...> {
		static void strideSum(size_t& stride, uint32_t& count) {
			VAOUpdater<Components...>::strideSum(stride, count);

			stride += sizeof(First);
			count += First::parts();
		}

		static void update(size_t const STRIDE, size_t offset = 0, int index = 0){
			// Push the component location
			First::push(offset, index, STRIDE);

			// Now recurse. Index and offset were modified. 
			VAOUpdater<Components...>::update(STRIDE, offset, index);
		}
	};

	class VAO : public BufferObj<VAO> {
	private:
		VBO vbo;
		EBO ebo;

		// count is Number of items per stride (args in push(...))

		/**
		 * @brief Generate the buffer object
		 * @return A reference to this buffer object
		 */
		virtual VAO& generate() override;
		
		/**
		 * @brief Destroy the buffer object
		 * @return A reference to this buffer object (ready for next)
		 */
		virtual VAO& destroy() override;	

	public:
		/**
		 * @brief Create an OpenGL Vertex Attribute Object.
		 */
		VAO();

		/**
		 * @brief Destroy the Vertex Attribute Object
		 */
		~VAO();
		
		/**
		 * @brief Get a reference to the VBO object
		 * @return A reference to the VBO object
		 */
		VBO& getVBO();
		
		/**
		 * @brief Get a reference to the EBO object
		 * @return A reference to the EBO object
		 */
		EBO& getEBO();
		
		/**
		 * @brief Bind the buffer.
		 * @return A reference to this buffer object
		 */
		virtual VAO& bind() override;

		/**
		 * @brief Unbind the VAO
		 */
		static void unbind();
		
		/**
		 * @brief Template variadic point push method
		 * @param[in] c		A variadic list of components
		 * @return		A reference to drawable object
		 */
		template <typename... Components>
		VAO& pushPoint(Components... c) {
			// Push in order...
			this->vbo.push(c...);

			return *this;
		}
		
		/**
		 * @brief Update the buffer object.
		 * @param[in] data	Ignored.
		 * @return A reference to this buffer object
		 */
		virtual VAO& update(void* data = nullptr) override {
			// Recursion termination case. No types specified.
			// Do nothing.
			return *this;
		}

		/**
		 * @brief Update the VAO given a static list of components
		 * @description	Sets getCount equal to the number of types provided
		 * @return	A reference to this VAO
		 */
		template <typename... Components>
		VAO& update() {
			// Does this need to happen first?
			this->bind();
			// Update Vertex Buffer Object
			this->vbo.update();
			// Update Entity Buffer Object
			// Will not do anything if no indices were pushed
			this->ebo.update();

			
			// First, recursively compute the stride
			size_t stride = 0;
			VAOUpdater<Components...>::strideSum(stride, this->count);

			std::cout << "Updating obj with stride " << stride << std::endl;

			// Then, recursively push the components in order
			VAOUpdater<Components...>::update(stride);

			// Unbind the vertex arrays
			VBO::unbind();
			VAO::unbind();
			return *this;
		}
	};
}

#endif
