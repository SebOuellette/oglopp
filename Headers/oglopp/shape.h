#ifndef OGLOPP_SHAPE_H
#define OGLOPP_SHAPE_H

#include <vector>
#include <glm/vec3.hpp>
#include <stdint.h>
#include <string>

#include "texture.h"
#include "window.h"
#include "shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace oglopp {
	/* @brief Shape object
	*/
	class Shape {
	protected:
		unsigned int vertCount = 0;
		unsigned int indexCount = 0;
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;

		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		unsigned int strideElements;
		unsigned int strideBytes;

		// The angle and position of this shape in the world.
		glm::vec3 scaleVec;
		glm::vec3 angle;
		glm::vec3 position;

		// Variables pre-defined for use in each draw() iteration
		int16_t size;
		uint16_t myRegister;

		/* @brief Get the opengl texture register for the n'th texture, where index = n
		 * @param[in]	index	The index/layer of the texture
		*/
		static uint16_t getTextureCode(uint8_t index);

		/* @brief Get the opengl texture string for the n'th texture, where index = n
		 * @param[in]	index	The index/layer of the texture
		*/
		static std::string getTextureString(uint8_t index);

		/* @brief Update uniform MVP
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	A pointer to the shader object
		 * @return				A reference to this shape object
		*/
		Shape& updateUniformMVP(Window& window, Shader* pShader);

		/* @brief Updated extra uniforms. obverloaded in each inherited class
		 * @return A reference to this shape object
	 	*/
		Shape& updateExtraUniforms();

		Shape& updateEBO();
		Shape& updateVBO();

	public:
		Shape();
		~Shape();

		/* @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @param[in]	col		The color of the vertex
		* @param[in]	texPos	The texture position
		* @param[in]	option	The option to push
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec3 col, glm::vec2 texPos, float option);

		/* @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @param[in]	col		The color of the vertex
		* @param[in]	texPos	The texture position
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec3 col, glm::vec2 texPos);

		/* @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @param[in]	col		The color of the vertex
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec3 col);

		/* @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @param[in]	texPos	The texture position
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec2 texPos);

		/* @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec);

		/* @brief Push a triangle to the indicies list. A triangle is constructed of the following verticies which were defined with pushPoint
		 * @param[in] vertA	The A vertex index out of the point list, where the first point is 0
		 * @param[in] vertB	The B vertex index
		 * @param[in] vertC	The C vertex index
		 * @return			A reference to this shape object
		*/
		Shape& pushTriangle(unsigned int vertA, unsigned int vertB, unsigned int vertC);

		/* @brief Push a texture onto the back of the texture stack.
		 * @param[in] newTexture	The texture object to set to
		 * @return					A reference to this shape object
		*/
		Shape& pushTexture(Texture const& newTexture);

		/* @brief Update the vertex, index, and texture coordinate list. Expected to be called when the texture list is modified.
		 * @param[in] color		Include the color/normal vec3
		 * @param[in] texture	Include the texture coord vec2
		 * @param[in] option	Include the option uint16_t
		 * @return	A reference to this shape object
		*/
		Shape& updateVAO(bool color = true, bool texture = true, bool option = false);

		unsigned int getVAO();
		unsigned int getVBO();
		std::vector<float>& getVertices();
		std::vector<Texture>& getTextureList();

		/* @brief Draw this shape to the specified window using an optional shader
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	An optional pointer to the shader object
		 * @return 				A reference to this shape
		*/
		Shape& draw(Window& window, Shader* pShader = nullptr);

		/* @brief Get the position of this shape
		 * @return The position of this shape
		*/
		glm::vec3 const& getPosition();

		/* @brief Get the angle of this shape
		 * @return The angle of this shape
		*/
		glm::vec3 const& getAngle();

		/* @brief Set the position of this shape in world space
		 * @param[in] newPosition	The position in world space
		 * @return					A reference to this position
		*/
		Shape& setPosition(glm::vec3 newPosition);

		/* @brief Set the angle of this shape in radians for each axis
		 * @param[in] newAngle		The angle to set to
		 * @return 					A reference to this shape object
		*/
		Shape& setAngle(glm::vec3 newAngle);

		/* @brief Translate this shape some coordinates in world space
		 * @param[in] offset	The offset to translate by
		 * @return				A reference to this shape object
		*/
		Shape& translate(glm::vec3 offset);

		/* @brief Rotate this shape around its local origin
		 * @param[in] offset	The offset to rotate by in radians for each axis
		 * @return				A reference to this shape object
		*/
		Shape& rotate(glm::vec3 offset);

		/* @brief Set the scale of the shape
		 * @param[in] newScale	The new scale for this shape
		 * @return 				A reference to this shape object
		*/
		Shape& setScale(glm::vec3 newScale);

		/* @brief Apply a scaling factor to the shape
		 * @param[in] offset	The new scale for this shape
		 * @return				A reference to this shape object
		*/
		Shape& scale(glm::vec3 offset);

		/* @brief Get the scale factor
		 * @return The scaling factor
		*/
		glm::vec3 const& getScale();
	};
}

#endif
