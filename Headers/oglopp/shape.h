#ifndef OGLOPP_SHAPE_H
#define OGLOPP_SHAPE_H

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

#include <iostream>

namespace oglopp {
	/** @brief Shape object
	*/
	class Shape {
	protected:
		unsigned int vertCount = 0;
		unsigned int indexCount = 0;
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;

		std::vector<uint8_t> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;

		unsigned int strideElements;
		unsigned int strideBytes;

		// The angle and position of this shape in the world.
		glm::dvec3 scaleVec;
		glm::dvec3 angle;
		glm::dvec3 position;

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

		/** @brief Update uniform MVP
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	A pointer to the shader object
		 * @return				A reference to this shape object
		*/
		Shape& updateUniformMVP(Window& window, Shader* pShader);

		/** @brief Updated extra uniforms. obverloaded in each inherited class
		 * @return A reference to this shape object
	 	*/
		Shape& updateExtraUniforms();

		Shape& updateEBO();
		Shape& updateVBO();

	public:
		enum DataType : uint16_t {
			FLOAT 	= GL_FLOAT,
			DOUBLE	= GL_DOUBLE,
			UINT8	= GL_UNSIGNED_BYTE,
			UINT16	= GL_UNSIGNED_SHORT,
			UINT32	= GL_UNSIGNED_INT,
			INT8	= GL_BYTE,
			INT16	= GL_SHORT,
			INT32	= GL_INT,
			// Float vec
			VEC2	= GL_FLOAT_VEC2,
			VEC3	= GL_FLOAT_VEC3,
			VEC4	= GL_FLOAT_VEC4,
			// Double vec
			DVEC2	= GL_DOUBLE_VEC2,
			DVEC3	= GL_DOUBLE_VEC3,
			DVEC4	= GL_DOUBLE_VEC4,
			// int32 vec
			IVEC2	= GL_INT_VEC2,
			IVEC3 	= GL_INT_VEC3,
			IVEC4	= GL_INT_VEC4,
			// int64 vec
			I64VEC2	= GL_INT64_VEC2_ARB,
			I64VEC3	= GL_INT64_VEC3_ARB,
			I64VEC4	= GL_INT64_VEC4_ARB,
			// uint32 vec
			UVEC2	= GL_UNSIGNED_INT_VEC2,
			UVEC3	= GL_UNSIGNED_INT_VEC3,
			UVEC4	= GL_UNSIGNED_INT_VEC4,
			// uint64 vec
			U64VEC2	= GL_UNSIGNED_INT64_VEC2_ARB,
			U64VEC3 = GL_UNSIGNED_INT64_VEC3_ARB,
			U64VEC4 = GL_UNSIGNED_INT64_VEC4_ARB
		};

		Shape();
		~Shape();

		/** @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @param[in]	col		The color of the vertex
		* @param[in]	texPos	The texture position
		* @param[in]	option	The option to push
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec3 col, glm::vec2 texPos, float option);

		/** @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @param[in]	col		The color of the vertex
		* @param[in]	texPos	The texture position
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec3 col, glm::vec2 texPos);

		/** @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @param[in]	col		The color of the vertex
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec3 col);

		/** @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @param[in]	texPos	The texture position
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec2 texPos);

		/** @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec);

		/** @brief Push a constant reference to some value. The size of the value must be able to be extracted from the type
		 * @param[in] pValue	A pointer to the value to push
		 * @param[in] bytes		The number of bytes pointer to by pValue
		 * @return 				A reference to this shape object
	 	*/
		Shape& pushValue(void const* pValue, size_t bytes);

		/** @brief Increment the number of vertices. Used when pushing template points to indicate the end of a vertex
		 * @return A reference to this shape
	 	*/
		Shape& incrementVerts();

		/** @brief Reset the vertex count
		 * @param[in] forceTo	Reset the vertex count to some value. Default is 0
		 * @return	A reference to this shape object
	 	*/
		Shape& resetVerts(unsigned int forceTo = 0);


		/** @brief Push a triangle to the indicies list. A triangle is constructed of the following verticies which were defined with pushPoint
		 * @param[in] vertA	The A vertex index out of the point list, where the first point is 0
		 * @param[in] vertB	The B vertex index
		 * @param[in] vertC	The C vertex index
		 * @return			A reference to this shape object
		*/
		Shape& pushTriangle(unsigned int vertA, unsigned int vertB, unsigned int vertC);

		/** @brief Push a texture onto the back of the texture stack.
		 * @param[in] newTexture	The texture object to set to
		 * @return					A reference to this shape object
		*/
		Shape& pushTexture(Texture* newTexture);

		/** @brief Update the vertex, index, and texture coordinate list. Expected to be called when the texture list is modified.
		 * @param[in] color		Include the color/normal vec3
		 * @param[in] texture	Include the texture coord vec2
		 * @param[in] option	Include the option uint16_t
		 * @return	A reference to this shape object
		*/
		Shape& updateVAO(bool color = true, bool texture = true, bool option = false);

		/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case
		 * @param[in] totalIndices	The total number of indices
		 * @return 	A reference to this shape object
	 	*/
		Shape& finalizePoints(const int totalIndices);

		/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case (DOES RECURSE)
		 * @param[in] index			The index of the point
		 * @param[in] firstParam	The first argument
		 * @param[in] args...		Variadic list of arguments. Arguments are of type Shape::DataType
		 * @return A reference to this shape object
	 	*/
		template <typename First, typename... Args>
		Shape& finalizePoints(const int index, First firstParam, Args...args) {
			// The current stride bytes is the offset
			const uint64_t OFFSET = this->strideBytes;

			// Calculate the stride elements and stride bytes for THIS SINGLE DATATYPE
			uint32_t thisStrideElems = Shape::getStrideElems(static_cast<DataType>(firstParam));
			uint32_t thisStrideBytes = thisStrideElems * Shape::getStrideComponentBytes(static_cast<DataType>(firstParam));

			//std::cout << "this bytes is " << thisStrideBytes << std::endl;

			// Accumulate this datatype's stride into the total
			this->strideElements += thisStrideElems;
			this->strideBytes += thisStrideBytes;

			// Recurse - accumulate all datatypes before using calculated total
			this->finalizePoints(index + 1, args...);

			// Now do what I gotta do - we can use the calculated total in this->strideElements and this->strideBytes
			switch(firstParam) {
				case FLOAT:
				case VEC2:
				case VEC3:
				case VEC4:
					glVertexAttribPointer(index, thisStrideElems, Shape::getStructComponentRegister(firstParam), GL_FALSE, this->strideBytes, (void*)OFFSET);
					break;

				case DVEC4:
				case DVEC3:
				case DVEC2:
				case DOUBLE:
					glVertexAttribLPointer(index, thisStrideElems, Shape::getStructComponentRegister(firstParam), this->strideBytes, (void*)OFFSET);
					break;

				case UINT8:
				case UINT16:
				case UINT32:
				case INT8:
				case INT16:
				case INT32:
				case IVEC2:
				case I64VEC2:
				case UVEC2:
				case U64VEC2:
				case IVEC3:
				case I64VEC3:
				case UVEC3:
				case U64VEC3:
				case IVEC4:
				case I64VEC4:
				case UVEC4:
				case U64VEC4:
					glVertexAttribIPointer(index, thisStrideElems, Shape::getStructComponentRegister(firstParam), this->strideBytes, (void*)OFFSET);
					break;

				default:
					return *this;
			}

			glEnableVertexAttribArray(index);

			return *this;
		}

		/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case (DOES RECURSE)
		 * @param[in] firstParam	The first argument
		 * @param[in] ...			Variadic list of arguments. Arguments are of type Shape::DataType
		 * @return A reference to this shape object
	 	*/
		template <typename First, typename... Args>
		Shape& finalizePoints(First firstParam, Args...args) {
			this->strideElements = 0;
			this->strideBytes = 0;

			finalizePoints(0, firstParam, args...);

			// Unbind the vertex array
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			return *this;
		}

		/** @brief Convert a shape DataType to a stride element count
		 * @param[in] dataType	The data type
		 * @return				The number of associated stride elements/components
	 	*/
		static const uint32_t getStrideElems(DataType const& dataType);

		/** @brief Get the number of bytes for a single stride component for some datatype
		 * @param[in] dataType	The data type
		 * @return				The number of associated stride elements/components
		*/
		static const uint32_t getStrideComponentBytes(DataType const& dataType);

		/** @brief Get the component register data type for some shape dataype
		 * @param[in] dataType	The shape datatype
		 * @return				The gl register
	 	*/
		static const uint32_t getStructComponentRegister(DataType const& dataType);

		unsigned int getVAO();
		unsigned int getVBO();
		std::vector<uint8_t>& getVertices();
		std::vector<Texture*>& getTextureList();

		/** @brief Draw this shape to the specified window using an optional shader
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	An optional pointer to the shader object
		 * @return 				A reference to this shape
		*/
		Shape& draw(Window& window, Shader* pShader = nullptr);

		/** @brief Get the position of this shape
		 * @return The position of this shape
		*/
		glm::dvec3 const& getPosition();

		/** @brief Get the angle of this shape
		 * @return The angle of this shape
		*/
		glm::dvec3 const& getAngle();

		/** @brief Set the position of this shape in world space
		 * @param[in] newPosition	The position in world space
		 * @return					A reference to this position
		*/
		Shape& setPosition(glm::dvec3 newPosition);

		/** @brief Set the angle of this shape in radians for each axis
		 * @param[in] newAngle		The angle to set to
		 * @return 					A reference to this shape object
		*/
		Shape& setAngle(glm::dvec3 newAngle);

		/** @brief Translate this shape some coordinates in world space
		 * @param[in] offset	The offset to translate by
		 * @return				A reference to this shape object
		*/
		Shape& translate(glm::dvec3 offset);

		/** @brief Rotate this shape around its local origin
		 * @param[in] offset	The offset to rotate by in radians for each axis
		 * @return				A reference to this shape object
		*/
		Shape& rotate(glm::dvec3 offset);

		/** @brief Set the scale of the shape
		 * @param[in] newScale	The new scale for this shape
		 * @return 				A reference to this shape object
		*/
		Shape& setScale(glm::dvec3 newScale);

		/** @brief Apply a scaling factor to the shape
		 * @param[in] offset	The new scale for this shape
		 * @return				A reference to this shape object
		*/
		Shape& scale(glm::dvec3 offset);

		/** @brief Get the scale factor
		 * @return The scaling factor
		*/
		glm::dvec3 const& getScale();
	};
}

#endif
