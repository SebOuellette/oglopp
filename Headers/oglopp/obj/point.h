#ifndef OGLOPP_POINT_H
#define OGLOPP_POINT_H

#include "oglopp/obj/component.h"


// This file has to go. This is not designed to store a contiguous set of data. I dunno man. Maybe I need to just make my own list class to serialize as it goes. 


namespace oglopp {
	struct Point {
	private:
		// Utils
		template <typename Callback>
		void foreach(Callback func) {
			for (size_t i = 0; i < sizeof(*this) / sizeof(void*); i++) {
				// If the pointer is not null, run the callback
				if ((&vert + i) != nullptr)
					func(static_cast<Component*>(static_cast<void*>(*(&vert + i))), i);
			}
		}
		
		// Components within a point. Each one is optional
		ComponentVertex vert = nullptr;
		ComponentNormal normal = nullptr;
		ComponentTextureCoordinate tex = nullptr;
		ComponentOption opt = nullptr;


	public:
		

		Point(int64_t vertIdx = -1, int64_t texIdx = -1, int64_t normIdx = -1, int64_t optIdx = -1) {
			vert = nullptr;
			normal = nullptr;
			tex = nullptr;
			opt = nullptr;

			// Verts
			if (vertIdx >= 0 && vertIdx < cVertices.size()) {
				vert = &cVertices[vertIdx];
			}

			// Texture coords
			if (texIdx >= 0 && texIdx < cTexCoords.size()) {
				tex = &cTexCoords[texIdx];
			}

			// Normal
			if (normIdx >= 0 && normIdx < cNormals.size()) {
				normal = &cNormals[normIdx];
			}

			// Option index
			if (optIdx >= 0 && optIdx < cOptions.size()) {
				opt = &cOptions[optIdx];
			}
		}

		/**
		 * @brief Compute the total number of bytes within the list of components. AKA the 'stride bytes'.
		 * @return The total number of bytes within the list of components
		 */
		inline size_t stride() {
			size_t sum = 0;

			foreach([&sum](Component* c, size_t idx){
				sum += c->size();
			});

			return sum;
		}

		/**
		 * @brief Compute the sum over the parts in the component
		 */
		inline size_t totalParts() {
			size_t sum = 0;

			foreach([&sum](Component* c, size_t idx) {
				sum += c->parts();
			});

			return sum;
		}
		
		/**
		 * @brief Assuming there is a bound VAO, push this->stide() bytes to the buffer to be passed to the GPU
		 * @param[out] index	A pointer to the index after pushing all elements (# of elements pushed - 1)
		 * @parma[out] offset	The offset after pushing all elements. (stride * elements)
		 */
		inline void pushVAOComponents(size_t* index = nullptr, size_t* offset = nullptr) {
			size_t myOffset = 0;
			const size_t MY_STRIDE = this->stride();

			foreach([&](Component* c, size_t idx) {
				glVertexAttribPointer(idx, c->parts(), c->type(), GL_FALSE, MY_STRIDE, (void*)myOffset);
				glEnableVertexAttribArray(idx);
				myOffset += c->size();

				// Save the index if pointers were provided
				if (index != nullptr) 
					*index = idx;
			});


			// Final offset
			if (offset != nullptr)
				*offset = myOffset;
		}

	};


}

#endif // OGLOPP_POINT_H 
