#ifndef OGLOPP_DRAWABLE_H
#define OGLOPP_DRAWABLE_H

#include <glm/ext/vector_float2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <cstdint>
#include <stdint.h>
#include <string>

#include "texture.h"
#include "window.h"
#include "shader.h"

#include "obj/vao.h"
#include "obj/projection.h"

namespace oglopp {
	/** @brief Drawable object
	*/
	class Shape : public VAO, public Projection {
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
		Shape();
		~Shape();	

		/** @brief Push a texture onto the back of the texture stack.
		 * @param[in] newTexture	The texture object to set to
		 * @return			A reference to this shape object
		*/
		Shape& pushTexture(Texture* newTexture);

		/**
		 * @brief Get a reference to the list of textures pushed
		 * @return	A reference to the list of textures pushed
		 */
		std::vector<Texture*> const& getTextureList();

		/** @brief Draw this shape to the specified window using an optional shader
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	An optional pointer to the shader object
		 * @return 		A reference to this shape
		*/
		virtual Shape& draw(Window& window, Shader* pShader = nullptr) override;
	};
}

#endif
