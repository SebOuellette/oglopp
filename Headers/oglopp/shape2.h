#ifndef OGLOPP_SHAPE2_H
#define OGLOPP_SHAPE2_H

#include <stdint.h>
#include <glm/ext/vector_float2.hpp>
//#include <stdexcept>
//#include <variant>
#include <vector>
#include <glm/vec3.hpp>
#include <stdint.h>
#include <string>

#include "oglopp/glad/gl.h"
#include "texture.h"
#include "window.h"
#include "shader.h"

#include <glm/gtc/matrix_transform.hpp>

//#include <iostream>
//#include <any>
//#include <concepts>

namespace oglopp {
	
	//	4am programming 
	//
	//template <typename L>
	//concept ComponentConcept = requires (L l, int& index, uint64_t& offset) {
	//	l.pushVAOIndex(index, offset);
	//};

	/** @brief Shape object
	*/
	class Shape2 {
	protected:

		
		

		/*
		class IComponentList {
		public:
			virtual ~IComponentList() = default;

			virtual inline size_t itemParts() const = 0;
			virtual inline int itemTypes() const = 0;
			virtual inline size_t itemBytes() const = 0;
			virtual inline size_t totalBytes() const = 0;
			virtual inline void pushVAOIndex(int& index, uint64_t& offset) const = 0;
		};

		template <typename T>
		class ComponentList : public IComponentList, public std::vector<T> {
		public:
			inline size_t itemParts() const override {
				return (!this->empty()) * T::parts;
			}

			inline int itemTypes() const override {
				return T::type;
			}

			inline size_t itemBytes() const override {
				return (!this->empty()) * sizeof(T);
			}

			inline size_t totalBytes() const override {
				return this->size() * this->itemBytes();
			}

			inline void pushVAOIndex(int& index, uint64_t& offset) const override {
				if (!this->empty()) {
					glBufferSubData(GL_ARRAY_BUFFER, offset, this->totalBytes(), this->data());
					glVertexAttribPointer(index, this->itemParts(), this->itemTypes(), GL_FALSE, 0, (void*)offset);
					glEnableVertexAttribArray(index);
					offset += this->totalBytes();
				}
				index++;
			}
		private:
		};*/ 

		
		typedef uint32_t Index;

		unsigned int indexCount = 0;
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;

		// Seperate list of vertex components
		std::vector<ComponentVertex> cVertices;
		std::vector<ComponentTextureCoordinate> cTexCoords;
		std::vector<ComponentNormal> cNormals;
		std::vector<ComponentOption> cOptions;

		std::vector<Point> pLists;
		std::vector<Index> indices;

		unsigned int strideElements;
		unsigned int strideBytes;

		// The angle and position of this shape in the world.
		glm::dvec3 scaleVec;
		glm::dvec3 angle;
		glm::dvec3 position;

		// Variables pre-defined for use in each draw() iteration
		int16_t size;
		uint16_t myRegister;
		std::vector<Texture*> textures;

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
		Shape2& updateUniformMVP(Window& window, Shader* pShader);

		/** @brief Updated extra uniforms. obverloaded in each inherited class
		 * @return A reference to this shape object
	 	*/
		Shape2& updateExtraUniforms();

		Shape2& updateEBO();
		Shape2& updateVBO();

		/*
		 * @brief Get the number of components for each vertex, in total. Update strideElements and strideBytes
		 * @return	The number of components for each vertex.
		 */
		inline uint32_t getComponentCount();

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

		Shape2();
		~Shape2();


		/**
		 * @brief Push a single vertex to the shape
		 * @param[in] vector	The vector of the vertex
		 * @return 				A reference to this shape object
		 */
		Shape2& pushVertex(glm::vec3 vector);

		/**
		 * @brief Push a single normal vector to the shape
		 * @param[in] normal	The normal vector to push
		 * @return 				A reference to this shape object
		 */
		Shape2& pushNormal(glm::vec3 normal);

		/**
		 * @brief Push a single texture coordinate to this shape
		 * @param[in] texCoord	The texture coordinate to push
		 * @return 				A reference to this shape object
		 */
		Shape2& pushTexCoord(glm::vec2 texCoord);

		/**
		 * @brief Push a single option to this shape
		 * @param[in] option	The option to push
		 * @return 				A reference to this shape object
		 */
		Shape2& pushOption(uint16_t option);


		/** @brief Push a triangle to the indicies list. A triangle is constructed of the following verticies which were defined with pushPoint
		 * @param[in] vertA	The A vertex index out of the point list, where the first point is 0
		 * @param[in] vertB	The B vertex index
		 * @param[in] vertC	The C vertex index
		 * @return			A reference to this shape object
		*/
		Shape2& pushTriangle(unsigned int vertA, unsigned int vertB, unsigned int vertC);

		/** @brief Push a texture onto the back of the texture stack.
		 * @param[in] newTexture	The texture object to set to
		 * @return					A reference to this shape object
		*/
		Shape2& pushTexture(Texture* newTexture);

		/** @brief Update the vertex, index, and texture coordinate list. Expected to be called when the texture list is modified.
		 * @return	A reference to this shape object
		*/
		Shape2& updateVAO();

		/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case
		 * @param[in] totalIndices	The total number of indices
		 * @return 	A reference to this shape object
	 	*/
		Shape2& finalizePoints(const int totalIndices);

		/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case (DOES RECURSE)
		 * @param[in] index			The index of the point
		 * @param[in] firstParam	The first argument
		 * @param[in] args...		Variadic list of arguments. Arguments are of type Shape::DataType
		 * @return A reference to this shape object
	 	*/
		template <typename First, typename... Args>
		Shape2& finalizePoints(const int index, First firstParam, Args...args) {
			// The current stride bytes is the offset
			const uint64_t OFFSET = this->strideBytes;

			// Calculate the stride elements and stride bytes for THIS SINGLE DATATYPE
			uint32_t thisStrideElems = Shape2::getStrideElems(static_cast<DataType>(firstParam));
			uint32_t thisStrideBytes = thisStrideElems * Shape2::getStrideComponentBytes(static_cast<DataType>(firstParam));

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
					glVertexAttribPointer(index, thisStrideElems, Shape2::getStructComponentRegister(firstParam), GL_FALSE, this->strideBytes, (void*)OFFSET);
					break;

				case DVEC4:
				case DVEC3:
				case DVEC2:
				case DOUBLE:
					glVertexAttribLPointer(index, thisStrideElems, Shape2::getStructComponentRegister(firstParam), this->strideBytes, (void*)OFFSET);
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
					glVertexAttribIPointer(index, thisStrideElems, Shape2::getStructComponentRegister(firstParam), this->strideBytes, (void*)OFFSET);
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
		Shape2& finalizePoints(First firstParam, Args...args) {
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
		std::vector<Texture*>& getTextureList();

		/** @brief Draw this shape to the specified window using an optional shader
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	An optional pointer to the shader object
		 * @return 				A reference to this shape
		*/
		Shape2& draw(Window& window, Shader* pShader = nullptr);

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
		Shape2& setPosition(glm::dvec3 newPosition);

		/** @brief Set the angle of this shape in radians for each axis
		 * @param[in] newAngle		The angle to set to
		 * @return 					A reference to this shape object
		*/
		Shape2& setAngle(glm::dvec3 newAngle);

		/** @brief Translate this shape some coordinates in world space
		 * @param[in] offset	The offset to translate by
		 * @return				A reference to this shape object
		*/
		Shape2& translate(glm::dvec3 offset);

		/** @brief Rotate this shape around its local origin
		 * @param[in] offset	The offset to rotate by in radians for each axis
		 * @return				A reference to this shape object
		*/
		Shape2& rotate(glm::dvec3 offset);

		/** @brief Set the scale of the shape
		 * @param[in] newScale	The new scale for this shape
		 * @return 				A reference to this shape object
		*/
		Shape2& setScale(glm::dvec3 newScale);

		/** @brief Apply a scaling factor to the shape
		 * @param[in] offset	The new scale for this shape
		 * @return				A reference to this shape object
		*/
		Shape2& scale(glm::dvec3 offset);

		/** @brief Get the scale factor
		 * @return The scaling factor
		*/
		glm::dvec3 const& getScale();
	};
}

#endif
