#ifndef OGLOPP_COMPONENT_H
#define OGLOPP_COMPONENT_H

#include "../defines.h"
#include <cstdio>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <initializer_list>
#include <iostream>

namespace oglopp {
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



	// CRTP (Curiously Recurring Template Pattern) design pattern
	// almost a decade of c++ experience and I still learn new things c:
	template <class Child>
	struct Component {
		// Implementations must define Child::parts and Child::type
		static constexpr uint32_t& parts = Child::parts;
		static constexpr DataType& type = Child::type;

		/**
		 * @brief Push the component to the bound attribute array
		 * @param[out] offset	The adjusted offset after pushing
		 * @param[out] index	The adjusted index after pushing 
		 * @param[in] stride	The number of bytes per stride
		 */
		static inline void push(size_t& offset, int& index, size_t stride) {
			// I hate opengl
			// We have to use one of these push calls based on the type provided. 
			switch(Child::type) {
				case FLOAT:
				case VEC2:
				case VEC3:
				case VEC4: {
					glVertexAttribPointer(index, 
						Child::parts, 
						Child::type, 
						GL_FALSE, 
						stride, 
						(void*)offset);
					break;
				}

				case DVEC4:
				case DVEC3:
				case DVEC2:
				case DOUBLE: {
					glVertexAttribLPointer(index, 
						Child::parts, 
						Child::type,
						stride,
						(void*)offset);
					break;
				}

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
				case U64VEC4: {
					glVertexAttribIPointer(index, 
						Child::parts, 
						Child::type, 
						stride, 
						(void*)offset);
					break;
				}
			}

			glEnableVertexAttribArray(index);
		
			offset += sizeof(Child);
			index++;
		}
	};

	/**
	 * @brief VAO vertex component.
	 */
	struct CVertex : public Component<CVertex> {
		static constexpr uint32_t parts = HLGL_VEC_COMPONENTS;
		static constexpr DataType type = DataType::FLOAT;

		float x;
		float y;
		float z;

		CVertex(float newX = 0, float newY = 0, float newZ = 0) : x(newX), y(newY), z(newZ) {}
		CVertex(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}
	};

	/**
	 * @brief VAO texture coordinate component
	 */
	struct CTexCoord : public Component<CTexCoord> {
		static constexpr uint32_t parts = HLGL_TEX_COMPONENTS;
		static constexpr DataType type = DataType::FLOAT;

		float x;
		float y;

		CTexCoord(float newX = 0, float newY = 0) : x(newX), y(newY) {}
		CTexCoord(glm::vec2 vec) : x(vec.x), y(vec.y) {}
	};

	/**
	 * @brief VAO normal vector component
	 */
	struct CNormal : public Component<CNormal> {
		static constexpr uint32_t parts = HLGL_NOR_COMPONENTS;
		static constexpr DataType type = DataType::FLOAT;

		float x;
		float y;
		float z;

		CNormal(float newX = 0, float newY = 0, float newZ = 0) : x(newX), y(newY), z(newZ) {}
		CNormal(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}
	};
}

#endif // OGLOPP_COMPONENT_H
