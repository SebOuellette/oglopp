#ifndef OGLOPP_DRAWABLE_H
#define OGLOPP_DRAWABLE_H

#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include <glm/vec3.hpp>
#include <stdint.h>
#include <string>

#include "oglopp/glad/gl.h"
#include "texture.h"
#include "window.h"
#include "shader.h"

#include <glm/gtc/matrix_transform.hpp>

#include "oglopp/obj/vao.h"
#include "oglopp/obj/projection.h"

namespace oglopp {
	/** @brief Drawable object
	*/
	class Drawable : public VAO, public Projection {
	protected:
		std::vector<Texture*> textures;

		// Variables pre-defined for use in each draw() iteration
		int16_t size;
		uint16_t myRegister;

		/** @brief Get the opengl texture register for the n'th texture, where index = n
		 * @param[in]	index	The index/layer of the texture
		*/
		static uint16_t getTextureCode(uint8_t index);

		/** @brief Get the opengl texture string for the n'th texture, where index = n
		 * @param[in]	index	The index/layer of the texture
		*/
		static std::string getTextureString(uint8_t index);

	public:	
		Drawable();
		~Drawable();

		/**
		 * @brief Template variadic point push method
		 * @param[in] c		A variadic list of components
		 * @return		A reference to drawable object
		 */
		template <typename... Components>
		Drawable& pushPoint(Components... c) {
			// Push in order...
			return this->getVBO().push(c...);
		}

		/** @brief Push a texture onto the back of the texture stack.
		 * @param[in] newTexture	The texture object to set to
		 * @return					A reference to this shape object
		*/
		Drawable& pushTexture(Texture* newTexture);

		std::vector<Texture*>& getTextureList();

		/** @brief Draw this shape to the specified window using an optional shader
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	An optional pointer to the shader object
		 * @return 				A reference to this shape
		*/
		Drawable& draw(Window& window, Shader* pShader = nullptr);
	};
}

#endif
