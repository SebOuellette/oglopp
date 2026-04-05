#ifndef OGLOPP_COMPONENT_H
#define OGLOPP_COMPONENT_H

#include "../defines.h"
#include <cstdio>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <initializer_list>
#include <iostream>

namespace oglopp {
	// CRTP (Curiously Recurring Template Pattern) design pattern
	// almost a decade of c++ experience and I still learn new things c:
	template <class Child>
	class Component {
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


		static inline const size_t parts() {
			return Child::parts();
		}

		static inline const int type() {
			return Child::type;
		}

		/**
		 * @brief Push the component to the bound attribute array
		 * @param[out] offset	The adjusted offset after pushing
		 * @param[out] index	The adjusted index after pushing 
		 * @param[in] stride	The number of bytes per stride
		 */
		static inline void push(size_t& offset, int& index, size_t stride) {
			glVertexAttribPointer(
				index, 
				Child::parts(), 
				Child::type(), 
				GL_FALSE, // do not perform normalization 
				stride, 
				(void*)offset
			);
			glEnableVertexAttribArray(index);
		
			offset += sizeof(Child);
			index++;
		}
	};

	/**
	 * @brief VAO vertex component.
	 */
	class CVertex : public Component<CVertex> {
	public:
		float x;
		float y;
		float z;

		CVertex(float newX = 0, float newY = 0, float newZ = 0) : x(newX), y(newY), z(newZ) {}
		CVertex(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}

		static inline const size_t parts() {
			return HLGL_VEC_COMPONENTS;
		}

		static inline const int type() {
			return DataType::FLOAT;
		}
	};

	/**
	 * @brief VAO texture coordinate component
	 */
	class CTexCoord : public Component<CTexCoord> {
	public:
		float x;
		float y;

		CTexCoord(float newX = 0, float newY = 0) : x(newX), y(newY) {}
		CTexCoord(glm::vec2 vec) : x(vec.x), y(vec.y) {}

		static inline const size_t parts() {
			return HLGL_TEX_COMPONENTS;
		}

		static inline const int type() {
			return DataType::FLOAT;
		}
	};

	/**
	 * @brief VAO normal vector component
	 */
	class CNormal : public Component<CNormal> {
	public:
		float x;
		float y;
		float z;

		CNormal(float newX = 0, float newY = 0, float newZ = 0) : x(newX), y(newY), z(newZ) {}
		CNormal(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}

		static inline const size_t parts() {
			return HLGL_NOR_COMPONENTS;
		}

		static inline const int type() {
			return DataType::FLOAT;
		}
	};
}

#endif // OGLOPP_COMPONENT_H
