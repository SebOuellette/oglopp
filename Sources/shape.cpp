#include <cstdint>
#include <glm/ext/vector_float2.hpp>

//#include "oglopp/defines.h"
#include "oglopp/shader.h"
#include "oglopp/shape.h"

namespace oglopp {

	/** @brief Get the opengl texture register for the n'th texture, where index = n
	* @param[in]	index	The index/layer of the texture
	*/
	uint16_t Shape::getTextureCode(uint8_t index) {
		return static_cast<uint16_t>(GL_TEXTURE0) + index;
	}

	/** @brief Get the opengl texture string for the n'th texture, where index = n
	* @param[in]	index	The index/layer of the texture
	*/
	std::string Shape::getTextureString(uint8_t index) {
		std::string str = "texture" + std::to_string(index);

		return str;
	}

	Shape::Shape() {
		// Initialize stuff
		this->size = 0;
		this->myRegister = 0;
	}

	Shape::~Shape() {}

	/** @brief Set the texture
	* @param[in] texture	The texture object to set to
	* @return		A reference to this shape object
	*/
	Shape& Shape::pushTexture(Texture* newTexture) {
		if (nullptr != newTexture) {
			this->textures.push_back(newTexture);
		}
		return *this;
	}
	
	/**
	 * @brief Get a reference to the list of textures pushed
	 * @return	A reference to the list of textures pushed
	 */
	std::vector<Texture*> const& Shape::getTextureList() {
		return this->textures;
	}

	/** @brief Draw this shape to the specified window using an optional shader
	* @param[in] window		A reference to the window object
	* @param[in] pShader	An optional pointer to the shader object
	* @return 				A reference to this shape
	*/
	Shape& Shape::draw(Window& window, Shader* pShader) {
		DrawType drawType = TRIANGLES;

		this->size = this->textures.size();

		// Do this stuff if the shader was specified
		if (pShader != nullptr) {
			drawType = pShader->getDrawType();

			// Use the shader for the following actions in this block...
			pShader->use();

			// Perform the model - view - projection calculation and pass through to the shader
			this->updateUniformMVP(window, pShader);

			// Apply textures to this object using the shader
			for (int16_t i=0;i<this->size;i++) {
				myRegister = Shape::getTextureCode(i);

				// Bind the texture. This is required to be performed for each frame, for each texture, for each object.
				this->textures[i]->bind(myRegister);

				// We want to re-set the texture string every frame in case the shader changes
				// There are much better ways to do this but it's just one line soo like...
				pShader->setInt(Shape::getTextureString(i), i);
			}
		}

		// Bind vertex array
		this->bind();	

		// this->getCount() = # of types given in latest vao.update<...>()  
		// vbo.getCount() = # of calls to vbo.push() (total strides)
		// ebo.getCount() = # of calls to ebo.push() (total sets of indices)

		// Draw
		switch (drawType) {
			default:
			case TRIANGLES: {
				if (this->getEBO().getCount() > 0) {
					glDrawElements(GL_TRIANGLES, this->getEBO().getCount(), GL_UNSIGNED_INT, 0);
				} else {
					glDrawArrays(GL_TRIANGLES, 0, this->getVBO().getCount());
				}
				break;
			}

			case LINE_LOOP:
			case LINE: {
				if (this->getEBO().getCount() > 0) {
					glDrawElements(GL_LINES, this->getEBO().getCount(), GL_UNSIGNED_INT, 0);
			 	} else {
			 		glDrawArrays((drawType == LINE) ? GL_LINE_STRIP : GL_LINE_LOOP, 0, this->getVBO().getCount());
			 	}
			 	break;
			 }

			case POINTS: {
				glDrawArrays(GL_POINTS, 0, this->getVBO().getCount());
				break;
			}
		}

		// Unbind vertex array
		this->unbind();

		return *this;
	}
}
