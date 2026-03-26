[33mcommit cde969d126b49ac9074632b288b720a46cea881d[m[33m ([m[1;36mHEAD[m[33m -> [m[1;32mobjloader[m[33m, [m[1;31mopengl/objloader[m[33m)[m
Author: SebOuellette <seb_ouellette@honeybeeks.net>
Date:   Sun Mar 15 23:15:36 2026 -0400

    Working on obj loader version of Shape class. Trying to make it more generalized and optimized (ideally)

[1mdiff --git a/.clangd b/.clangd[m
[1mindex 5c95cd9..c0daa15 100644[m
[1m--- a/.clangd[m
[1m+++ b/.clangd[m
[36m@@ -1,4 +1,5 @@[m
 CompileFlags:[m
 	Add:[m
[32m+[m		[32m- "-std=c++20"[m
 		- "-I/network/Programming/opengl/Headers/"[m
 		- "-I/network/Programming/usr/include/"[m
[1mdiff --git a/CMakeLists.txt b/CMakeLists.txt[m
[1mindex 4362c83..2db3100 100644[m
[1m--- a/CMakeLists.txt[m
[1m+++ b/CMakeLists.txt[m
[36m@@ -1,9 +1,12 @@[m
 cmake_minimum_required(VERSION 4.0.1)[m
 project(oglopp[m
[31m-	VERSION 1.0.5[m
[32m+[m	[32mVERSION 1.0.6[m
 	LANGUAGES C CXX[m
 )[m
 [m
[32m+[m[32mset(CMAKE_CXX_STANDARD 20)[m
[32m+[m[32mset(CMAKE_CXX_STANDARD_REQUIRED ON)[m
[32m+[m
 # PIC everywhere[m
 set(CMAKE_POSITION_INDEPENDENT_CODE ON)[m
 set(BUILD_SHARED_LIBS ON)[m
[1mdiff --git a/Headers/oglopp/defines.h b/Headers/oglopp/defines.h[m
[1mindex 9cdc253..ee2b5f6 100644[m
[1m--- a/Headers/oglopp/defines.h[m
[1m+++ b/Headers/oglopp/defines.h[m
[36m@@ -19,11 +19,13 @@[m
 #define HLGL_WORLD_UP 		glm::dvec3(0.0f, 1.0f, 0.0f)[m
 [m
 #define HLGL_VEC_COMPONENTS 	3[m
[31m-#define HLGL_EBO_COMPONENTS		HLGL_VEC_COMPONENTS // One EBO index is composed of this many integers[m
[31m-#define HLGL_COL_COMPONENTS 	3[m
 #define HLGL_TEX_COMPONENTS		2[m
[32m+[m[32m#define HLGL_COL_COMPONENTS 	3[m
[32m+[m[32m#define HLGL_NOR_COMPONENTS		HLGL_COL_COMPONENTS[m
 #define HLGL_OPT_COMPONENTS		1[m
 [m
[32m+[m[32m#define HLGL_EBO_COMPONENTS		HLGL_VEC_COMPONENTS // One EBO index is composed of this many integers[m
[32m+[m
 #define HLGL_RENDER_FAR			(800.f)[m
 #define HLGL_RENDER_NEAR		(0.01f)[m
 [m
[1mdiff --git a/Headers/oglopp/more_shapes.h b/Headers/oglopp/more_shapes.h[m
[1mindex d72d104..5b24776 100644[m
[1m--- a/Headers/oglopp/more_shapes.h[m
[1m+++ b/Headers/oglopp/more_shapes.h[m
[36m@@ -2,6 +2,7 @@[m
 #define OGLOPP_MORE_SHAPES_H[m
 [m
 #include "shape.h"[m
[32m+[m[32m#include "shape2.h"[m
 [m
 [m
 #define MAP_TO_COORD2(vertIndex, unitSize) (((static_cast<double>(vertIndex) * static_cast<double>(unitSize)) * 2) - 1.0)[m
[36m@@ -28,6 +29,13 @@[m [mnamespace oglopp {[m
 		Cube();[m
 	};[m
 [m
[32m+[m	[32m/** @brief 3D Cube object[m
[32m+[m	[32m*/[m
[32m+[m	[32mclass Cube2 : public Shape2 {[m
[32m+[m	[32mpublic:[m
[32m+[m		[32mCube2();[m
[32m+[m	[32m};[m
[32m+[m
 	/** @brief 3D Sphere object[m
 	*/[m
 	class Sphere : public Shape {[m
[1mdiff --git a/Headers/oglopp/shape2.h b/Headers/oglopp/shape2.h[m
[1mnew file mode 100644[m
[1mindex 0000000..d485d6c[m
[1m--- /dev/null[m
[1m+++ b/Headers/oglopp/shape2.h[m
[36m@@ -0,0 +1,493 @@[m
[32m+[m[32m#ifndef OGLOPP_SHAPE2_H[m
[32m+[m[32m#define OGLOPP_SHAPE2_H[m
[32m+[m
[32m+[m[32m#include <cstdint>[m
[32m+[m[32m#include <glm/ext/vector_float2.hpp>[m
[32m+[m[32m#include <stdexcept>[m
[32m+[m[32m#include <variant>[m
[32m+[m[32m#include <vector>[m
[32m+[m[32m#include <glm/vec3.hpp>[m
[32m+[m[32m#include <stdint.h>[m
[32m+[m[32m#include <string>[m
[32m+[m
[32m+[m[32m#include "oglopp/glad/gl.h"[m
[32m+[m[32m#include "texture.h"[m
[32m+[m[32m#include "window.h"[m
[32m+[m[32m#include "shader.h"[m
[32m+[m
[32m+[m[32m#include <glm/gtc/matrix_transform.hpp>[m
[32m+[m
[32m+[m[32m#include <iostream>[m
[32m+[m[32m#include <any>[m
[32m+[m[32m#include <concepts>[m
[32m+[m
[32m+[m[32mnamespace oglopp {[m
[32m+[m
[32m+[m	[32mtemplate <typename L>[m
[32m+[m	[32mconcept ComponentConcept = requires (L l, int& index, uint64_t& offset) {[m
[32m+[m		[32ml.pushVAOIndex(index, offset);[m
[32m+[m	[32m};[m
[32m+[m
[32m+[m	[32m/** @brief Shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mclass Shape2 {[m
[32m+[m	[32mprotected:[m
[32m+[m
[32m+[m		[32mstruct Component {[m
[32m+[m			[32mstatic constexpr size_t parts = 0;[m
[32m+[m			[32mstatic constexpr int type = 0;[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32m/**[m
[32m+[m		[32m * @brief VAO vertex component.[m
[32m+[m		[32m */[m
[32m+[m		[32mstruct ComponentVertex : public Component {[m
[32m+[m			[32mfloat x;[m
[32m+[m			[32mfloat y;[m
[32m+[m			[32mfloat z;[m
[32m+[m
[32m+[m			[32mstatic constexpr size_t parts = 3;[m
[32m+[m			[32mstatic constexpr int type = GL_FLOAT;[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32m/**[m
[32m+[m		[32m * @brief VAO texture coordinate component[m
[32m+[m		[32m */[m
[32m+[m		[32mstruct ComponentTextureCoordinate : public Component {[m
[32m+[m			[32mfloat x;[m
[32m+[m			[32mfloat y;[m
[32m+[m
[32m+[m			[32mstatic constexpr size_t parts = 2;[m
[32m+[m			[32mstatic constexpr int type = GL_FLOAT;[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32m/**[m
[32m+[m		[32m * @brief VAO normal vector component[m
[32m+[m		[32m */[m
[32m+[m		[32mstruct ComponentNormal : public Component {[m
[32m+[m			[32mfloat x;[m
[32m+[m			[32mfloat y;[m
[32m+[m			[32mfloat z;[m
[32m+[m
[32m+[m			[32mstatic constexpr size_t parts = 3;[m
[32m+[m			[32mstatic constexpr int type = GL_FLOAT;[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32m/**[m
[32m+[m		[32m * @brief VAO option vector component. Surface type, bit mask, etc. For any custom use.[m
[32m+[m		[32m */[m
[32m+[m		[32mstruct ComponentOption : public Component {[m
[32m+[m			[32muint16_t opt;[m
[32m+[m
[32m+[m			[32mstatic constexpr size_t parts = 1;[m
[32m+[m			[32mstatic constexpr int type = GL_UNSIGNED_INT16_NV;[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m
[32m+[m		[32mclass IComponentList {[m
[32m+[m		[32mpublic:[m
[32m+[m			[32mvirtual ~IComponentList() = default;[m
[32m+[m
[32m+[m			[32mvirtual inline size_t itemParts() const = 0;[m
[32m+[m			[32mvirtual inline int itemTypes() const = 0;[m
[32m+[m			[32mvirtual inline size_t itemBytes() const = 0;[m
[32m+[m			[32mvirtual inline size_t totalBytes() const = 0;[m
[32m+[m			[32mvirtual inline void pushVAOIndex(int& index, uint64_t& offset) const = 0;[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32mtemplate <typename T>[m
[32m+[m		[32mclass ComponentList : public IComponentList, public std::vector<T> {[m
[32m+[m		[32mpublic:[m
[32m+[m			[32minline size_t itemParts() const override {[m
[32m+[m				[32mreturn (!this->empty()) * T::parts;[m
[32m+[m			[32m}[m
[32m+[m
[32m+[m			[32minline int itemTypes() const override {[m
[32m+[m				[32mreturn T::type;[m
[32m+[m			[32m}[m
[32m+[m
[32m+[m			[32minline size_t itemBytes() const override {[m
[32m+[m				[32mreturn (!this->empty()) * sizeof(T);[m
[32m+[m			[32m}[m
[32m+[m
[32m+[m			[32minline size_t totalBytes() const override {[m
[32m+[m				[32mreturn this->size() * this->itemBytes();[m
[32m+[m			[32m}[m
[32m+[m
[32m+[m			[32minline void pushVAOIndex(int& index, uint64_t& offset) const override {[m
[32m+[m				[32mif (!this->empty()) {[m
[32m+[m					[32mglBufferSubData(GL_ARRAY_BUFFER, offset, this->totalBytes(), this->data());[m
[32m+[m					[32mglVertexAttribPointer(index, this->itemParts(), this->itemTypes(), GL_FALSE, 0, (void*)offset);[m
[32m+[m					[32mglEnableVertexAttribArray(index);[m
[32m+[m					[32moffset += this->totalBytes();[m
[32m+[m				[32m}[m
[32m+[m				[32mindex++;[m
[32m+[m			[32m}[m
[32m+[m		[32mprivate:[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32mclass ComponentLists : public std::vector<IComponentList*> {[m
[32m+[m		[32mpublic:[m
[32m+[m			[32minline size_t totalByteSum() {[m
[32m+[m				[32msize_t sum = 0;[m
[32m+[m
[32m+[m				[32mfor (auto& item : *this) {[m
[32m+[m					[32msum += item->totalBytes();[m
[32m+[m				[32m}[m
[32m+[m
[32m+[m				[32mreturn sum;[m
[32m+[m			[32m}[m
[32m+[m
[32m+[m			[32minline size_t itemPartSum() {[m
[32m+[m				[32msize_t sum = 0;[m
[32m+[m
[32m+[m				[32mfor (auto& item : *this) {[m
[32m+[m					[32msum += item->itemParts();[m
[32m+[m				[32m}[m
[32m+[m
[32m+[m				[32mreturn sum;[m
[32m+[m			[32m}[m
[32m+[m
[32m+[m			[32minline size_t itemByteSum() {[m
[32m+[m				[32msize_t sum = 0;[m
[32m+[m
[32m+[m				[32mfor (auto& item : *this) {[m
[32m+[m					[32msum += item->itemBytes();[m
[32m+[m				[32m}[m
[32m+[m
[32m+[m				[32mreturn sum;[m
[32m+[m			[32m}[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32mtypedef uint32_t Index;[m
[32m+[m
[32m+[m		[32mclass ComponentIndices : public std::vector<Index> {[m
[32m+[m
[32m+[m			[32m//TODO[m
[32m+[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32munsigned int indexCount = 0;[m
[32m+[m		[32munsigned int VAO = 0;[m
[32m+[m		[32munsigned int VBO = 0;[m
[32m+[m		[32munsigned int EBO = 0;[m
[32m+[m
[32m+[m		[32m// Seperate list of vertex components[m
[32m+[m		[32mComponentList<ComponentVertex> cVertices;[m
[32m+[m		[32mComponentList<ComponentTextureCoordinate> cTexCoords;[m
[32m+[m		[32mComponentList<ComponentNormal> cNormals;[m
[32m+[m		[32mComponentList<ComponentOption> cOptions;[m
[32m+[m
[32m+[m		[32mComponentLists cLists;[m
[32m+[m		[32mstd::vector<Index> indices;[m
[32m+[m
[32m+[m		[32munsigned int strideElements;[m
[32m+[m		[32munsigned int strideBytes;[m
[32m+[m
[32m+[m		[32m// The angle and position of this shape in the world.[m
[32m+[m		[32mglm::dvec3 scaleVec;[m
[32m+[m		[32mglm::dvec3 angle;[m
[32m+[m		[32mglm::dvec3 position;[m
[32m+[m
[32m+[m		[32m// Variables pre-defined for use in each draw() iteration[m
[32m+[m		[32mint16_t size;[m
[32m+[m		[32muint16_t myRegister;[m
[32m+[m		[32mstd::vector<Texture*> textures;[m
[32m+[m
[32m+[m		[32m/** @brief Get the opengl texture register for the n'th texture, where index = n[m
[32m+[m		[32m * @param[in]	index	The index/layer of the texture[m
[32m+[m		[32m*/[m
[32m+[m		[32mstatic uint16_t getTextureCode(uint8_t index);[m
[32m+[m
[32m+[m		[32m/** @brief Get the opengl texture string for the n'th texture, where index = n[m
[32m+[m		[32m * @param[in]	index	The index/layer of the texture[m
[32m+[m		[32m*/[m
[32m+[m		[32mstatic std::string getTextureString(uint8_t index);[m
[32m+[m
[32m+[m		[32m/** @brief Update uniform MVP[m
[32m+[m		[32m * @param[in] window	A reference to the window object[m
[32m+[m		[32m * @param[in] pShader	A pointer to the shader object[m
[32m+[m		[32m * @return				A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& updateUniformMVP(Window& window, Shader* pShader);[m
[32m+[m
[32m+[m		[32m/** @brief Updated extra uniforms. obverloaded in each inherited class[m
[32m+[m		[32m * @return A reference to this shape object[m
[32m+[m	[41m [m	[32m*/[m
[32m+[m		[32mShape2& updateExtraUniforms();[m
[32m+[m
[32m+[m		[32mShape2& updateEBO();[m
[32m+[m		[32mShape2& updateVBO();[m
[32m+[m
[32m+[m		[32m/*[m
[32m+[m		[32m * @brief Get the number of components for each vertex, in total. Update strideElements and strideBytes[m
[32m+[m		[32m * @return	The number of components for each vertex.[m
[32m+[m		[32m */[m
[32m+[m		[32minline uint32_t getComponentCount();[m
[32m+[m
[32m+[m	[32mpublic:[m
[32m+[m		[32menum DataType : uint16_t {[m
[32m+[m			[32mFLOAT 	= GL_FLOAT,[m
[32m+[m			[32mDOUBLE	= GL_DOUBLE,[m
[32m+[m			[32mUINT8	= GL_UNSIGNED_BYTE,[m
[32m+[m			[32mUINT16	= GL_UNSIGNED_SHORT,[m
[32m+[m			[32mUINT32	= GL_UNSIGNED_INT,[m
[32m+[m			[32mINT8	= GL_BYTE,[m
[32m+[m			[32mINT16	= GL_SHORT,[m
[32m+[m			[32mINT32	= GL_INT,[m
[32m+[m			[32m// Float vec[m
[32m+[m			[32mVEC2	= GL_FLOAT_VEC2,[m
[32m+[m			[32mVEC3	= GL_FLOAT_VEC3,[m
[32m+[m			[32mVEC4	= GL_FLOAT_VEC4,[m
[32m+[m			[32m// Double vec[m
[32m+[m			[32mDVEC2	= GL_DOUBLE_VEC2,[m
[32m+[m			[32mDVEC3	= GL_DOUBLE_VEC3,[m
[32m+[m			[32mDVEC4	= GL_DOUBLE_VEC4,[m
[32m+[m			[32m// int32 vec[m
[32m+[m			[32mIVEC2	= GL_INT_VEC2,[m
[32m+[m			[32mIVEC3 	= GL_INT_VEC3,[m
[32m+[m			[32mIVEC4	= GL_INT_VEC4,[m
[32m+[m			[32m// int64 vec[m
[32m+[m			[32mI64VEC2	= GL_INT64_VEC2_ARB,[m
[32m+[m			[32mI64VEC3	= GL_INT64_VEC3_ARB,[m
[32m+[m			[32mI64VEC4	= GL_INT64_VEC4_ARB,[m
[32m+[m			[32m// uint32 vec[m
[32m+[m			[32mUVEC2	= GL_UNSIGNED_INT_VEC2,[m
[32m+[m			[32mUVEC3	= GL_UNSIGNED_INT_VEC3,[m
[32m+[m			[32mUVEC4	= GL_UNSIGNED_INT_VEC4,[m
[32m+[m			[32m// uint64 vec[m
[32m+[m			[32mU64VEC2	= GL_UNSIGNED_INT64_VEC2_ARB,[m
[32m+[m			[32mU64VEC3 = GL_UNSIGNED_INT64_VEC3_ARB,[m
[32m+[m			[32mU64VEC4 = GL_UNSIGNED_INT64_VEC4_ARB[m
[32m+[m		[32m};[m
[32m+[m
[32m+[m		[32mShape2();[m
[32m+[m		[32m~Shape2();[m
[32m+[m
[32m+[m
[32m+[m		[32m/**[m
[32m+[m		[32m * @brief Push a single vertex to the shape[m
[32m+[m		[32m * @param[in] vector	The vector of the vertex[m
[32m+[m		[32m * @return 				A reference to this shape object[m
[32m+[m		[32m */[m
[32m+[m		[32mShape2& pushVertex(glm::vec3 vector);[m
[32m+[m
[32m+[m		[32m/**[m
[32m+[m		[32m * @brief Push a single normal vector to the shape[m
[32m+[m		[32m * @param[in] normal	The normal vector to push[m
[32m+[m		[32m * @return 				A reference to this shape object[m
[32m+[m		[32m */[m
[32m+[m		[32mShape2& pushNormal(glm::vec3 normal);[m
[32m+[m
[32m+[m		[32m/**[m
[32m+[m		[32m * @brief Push a single texture coordinate to this shape[m
[32m+[m		[32m * @param[in] texCoord	The texture coordinate to push[m
[32m+[m		[32m * @return 				A reference to this shape object[m
[32m+[m		[32m */[m
[32m+[m		[32mShape2& pushTexCoord(glm::vec2 texCoord);[m
[32m+[m
[32m+[m		[32m/**[m
[32m+[m		[32m * @brief Push a single option to this shape[m
[32m+[m		[32m * @param[in] option	The option to push[m
[32m+[m		[32m * @return 				A reference to this shape object[m
[32m+[m		[32m */[m
[32m+[m		[32mShape2& pushOption(uint16_t option);[m
[32m+[m
[32m+[m
[32m+[m		[32m/** @brief Push a triangle to the indicies list. A triangle is constructed of the following verticies which were defined with pushPoint[m
[32m+[m		[32m * @param[in] vertA	The A vertex index out of the point list, where the first point is 0[m
[32m+[m		[32m * @param[in] vertB	The B vertex index[m
[32m+[m		[32m * @param[in] vertC	The C vertex index[m
[32m+[m		[32m * @return			A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& pushTriangle(unsigned int vertA, unsigned int vertB, unsigned int vertC);[m
[32m+[m
[32m+[m		[32m/** @brief Push a texture onto the back of the texture stack.[m
[32m+[m		[32m * @param[in] newTexture	The texture object to set to[m
[32m+[m		[32m * @return					A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& pushTexture(Texture* newTexture);[m
[32m+[m
[32m+[m		[32m/** @brief Update the vertex, index, and texture coordinate list. Expected to be called when the texture list is modified.[m
[32m+[m		[32m * @return	A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& updateVAO();[m
[32m+[m
[32m+[m		[32m/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case[m
[32m+[m		[32m * @param[in] totalIndices	The total number of indices[m
[32m+[m		[32m * @return 	A reference to this shape object[m
[32m+[m	[41m [m	[32m*/[m
[32m+[m		[32mShape2& finalizePoints(const int totalIndices);[m
[32m+[m
[32m+[m		[32m/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case (DOES RECURSE)[m
[32m+[m		[32m * @param[in] index			The index of the point[m
[32m+[m		[32m * @param[in] firstParam	The first argument[m
[32m+[m		[32m * @param[in] args...		Variadic list of arguments. Arguments are of type Shape::DataType[m
[32m+[m		[32m * @return A reference to this shape object[m
[32m+[m	[41m [m	[32m*/[m
[32m+[m		[32mtemplate <typename First, typename... Args>[m
[32m+[m		[32mShape2& finalizePoints(const int index, First firstParam, Args...args) {[m
[32m+[m			[32m// The current stride bytes is the offset[m
[32m+[m			[32mconst uint64_t OFFSET = this->strideBytes;[m
[32m+[m
[32m+[m			[32m// Calculate the stride elements and stride bytes for THIS SINGLE DATATYPE[m
[32m+[m			[32muint32_t thisStrideElems = Shape2::getStrideElems(static_cast<DataType>(firstParam));[m
[32m+[m			[32muint32_t thisStrideBytes = thisStrideElems * Shape2::getStrideComponentBytes(static_cast<DataType>(firstParam));[m
[32m+[m
[32m+[m			[32m//std::cout << "this bytes is " << thisStrideBytes << std::endl;[m
[32m+[m
[32m+[m			[32m// Accumulate this datatype's stride into the total[m
[32m+[m			[32mthis->strideElements += thisStrideElems;[m
[32m+[m			[32mthis->strideBytes += thisStrideBytes;[m
[32m+[m
[32m+[m			[32m// Recurse - accumulate all datatypes before using calculated total[m
[32m+[m			[32mthis->finalizePoints(index + 1, args...);[m
[32m+[m
[32m+[m			[32m// Now do what I gotta do - we can use the calculated total in this->strideElements and this->strideBytes[m
[32m+[m			[32mswitch(firstParam) {[m
[32m+[m				[32mcase FLOAT:[m
[32m+[m				[32mcase VEC2:[m
[32m+[m				[32mcase VEC3:[m
[32m+[m				[32mcase VEC4:[m
[32m+[m					[32mglVertexAttribPointer(index, thisStrideElems, Shape2::getStructComponentRegister(firstParam), GL_FALSE, this->strideBytes, (void*)OFFSET);[m
[32m+[m					[32mbreak;[m
[32m+[m
[32m+[m				[32mcase DVEC4:[m
[32m+[m				[32mcase DVEC3:[m
[32m+[m				[32mcase DVEC2:[m
[32m+[m				[32mcase DOUBLE:[m
[32m+[m					[32mglVertexAttribLPointer(index, thisStrideElems, Shape2::getStructComponentRegister(firstParam), this->strideBytes, (void*)OFFSET);[m
[32m+[m					[32mbreak;[m
[32m+[m
[32m+[m				[32mcase UINT8:[m
[32m+[m				[32mcase UINT16:[m
[32m+[m				[32mcase UINT32:[m
[32m+[m				[32mcase INT8:[m
[32m+[m				[32mcase INT16:[m
[32m+[m				[32mcase INT32:[m
[32m+[m				[32mcase IVEC2:[m
[32m+[m				[32mcase I64VEC2:[m
[32m+[m				[32mcase UVEC2:[m
[32m+[m				[32mcase U64VEC2:[m
[32m+[m				[32mcase IVEC3:[m
[32m+[m				[32mcase I64VEC3:[m
[32m+[m				[32mcase UVEC3:[m
[32m+[m				[32mcase U64VEC3:[m
[32m+[m				[32mcase IVEC4:[m
[32m+[m				[32mcase I64VEC4:[m
[32m+[m				[32mcase UVEC4:[m
[32m+[m				[32mcase U64VEC4:[m
[32m+[m					[32mglVertexAttribIPointer(index, thisStrideElems, Shape2::getStructComponentRegister(firstParam), this->strideBytes, (void*)OFFSET);[m
[32m+[m					[32mbreak;[m
[32m+[m
[32m+[m				[32mdefault:[m
[32m+[m					[32mreturn *this;[m
[32m+[m			[32m}[m
[32m+[m
[32m+[m			[32mglEnableVertexAttribArray(index);[m
[32m+[m
[32m+[m			[32mreturn *this;[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32m/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case (DOES RECURSE)[m
[32m+[m		[32m * @param[in] firstParam	The first argument[m
[32m+[m		[32m * @param[in] ...			Variadic list of arguments. Arguments are of type Shape::DataType[m
[32m+[m		[32m * @return A reference to this shape object[m
[32m+[m	[41m [m	[32m*/[m
[32m+[m		[32mtemplate <typename First, typename... Args>[m
[32m+[m		[32mShape2& finalizePoints(First firstParam, Args...args) {[m
[32m+[m			[32mthis->strideElements = 0;[m
[32m+[m			[32mthis->strideBytes = 0;[m
[32m+[m
[32m+[m			[32mfinalizePoints(0, firstParam, args...);[m
[32m+[m
[32m+[m			[32m// Unbind the vertex array[m
[32m+[m			[32mglBindBuffer(GL_ARRAY_BUFFER, 0);[m
[32m+[m			[32mglBindVertexArray(0);[m
[32m+[m
[32m+[m			[32mreturn *this;[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32m/** @brief Convert a shape DataType to a stride element count[m
[32m+[m		[32m * @param[in] dataType	The data type[m
[32m+[m		[32m * @return				The number of associated stride elements/components[m
[32m+[m	[41m [m	[32m*/[m
[32m+[m		[32mstatic const uint32_t getStrideElems(DataType const& dataType);[m
[32m+[m
[32m+[m		[32m/** @brief Get the number of bytes for a single stride component for some datatype[m
[32m+[m		[32m * @param[in] dataType	The data type[m
[32m+[m		[32m * @return				The number of associated stride elements/components[m
[32m+[m		[32m*/[m
[32m+[m		[32mstatic const uint32_t getStrideComponentBytes(DataType const& dataType);[m
[32m+[m
[32m+[m		[32m/** @brief Get the component register data type for some shape dataype[m
[32m+[m		[32m * @param[in] dataType	The shape datatype[m
[32m+[m		[32m * @return				The gl register[m
[32m+[m	[41m [m	[32m*/[m
[32m+[m		[32mstatic const uint32_t getStructComponentRegister(DataType const& dataType);[m
[32m+[m
[32m+[m		[32munsigned int getVAO();[m
[32m+[m		[32munsigned int getVBO();[m
[32m+[m		[32mstd::vector<Texture*>& getTextureList();[m
[32m+[m
[32m+[m		[32m/** @brief Draw this shape to the specified window using an optional shader[m
[32m+[m		[32m * @param[in] window	A reference to the window object[m
[32m+[m		[32m * @param[in] pShader	An optional pointer to the shader object[m
[32m+[m		[32m * @return 				A reference to this shape[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& draw(Window& window, Shader* pShader = nullptr);[m
[32m+[m
[32m+[m		[32m/** @brief Get the position of this shape[m
[32m+[m		[32m * @return The position of this shape[m
[32m+[m		[32m*/[m
[32m+[m		[32mglm::dvec3 const& getPosition();[m
[32m+[m
[32m+[m		[32m/** @brief Get the angle of this shape[m
[32m+[m		[32m * @return The angle of this shape[m
[32m+[m		[32m*/[m
[32m+[m		[32mglm::dvec3 const& getAngle();[m
[32m+[m
[32m+[m		[32m/** @brief Set the position of this shape in world space[m
[32m+[m		[32m * @param[in] newPosition	The position in world space[m
[32m+[m		[32m * @return					A reference to this position[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& setPosition(glm::dvec3 newPosition);[m
[32m+[m
[32m+[m		[32m/** @brief Set the angle of this shape in radians for each axis[m
[32m+[m		[32m * @param[in] newAngle		The angle to set to[m
[32m+[m		[32m * @return 					A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& setAngle(glm::dvec3 newAngle);[m
[32m+[m
[32m+[m		[32m/** @brief Translate this shape some coordinates in world space[m
[32m+[m		[32m * @param[in] offset	The offset to translate by[m
[32m+[m		[32m * @return				A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& translate(glm::dvec3 offset);[m
[32m+[m
[32m+[m		[32m/** @brief Rotate this shape around its local origin[m
[32m+[m		[32m * @param[in] offset	The offset to rotate by in radians for each axis[m
[32m+[m		[32m * @return				A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& rotate(glm::dvec3 offset);[m
[32m+[m
[32m+[m		[32m/** @brief Set the scale of the shape[m
[32m+[m		[32m * @param[in] newScale	The new scale for this shape[m
[32m+[m		[32m * @return 				A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& setScale(glm::dvec3 newScale);[m
[32m+[m
[32m+[m		[32m/** @brief Apply a scaling factor to the shape[m
[32m+[m		[32m * @param[in] offset	The new scale for this shape[m
[32m+[m		[32m * @return				A reference to this shape object[m
[32m+[m		[32m*/[m
[32m+[m		[32mShape2& scale(glm::dvec3 offset);[m
[32m+[m
[32m+[m		[32m/** @brief Get the scale factor[m
[32m+[m		[32m * @return The scaling factor[m
[32m+[m		[32m*/[m
[32m+[m		[32mglm::dvec3 const& getScale();[m
[32m+[m	[32m};[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m#endif[m
[1mdiff --git a/Sources/more_shapes.cpp b/Sources/more_shapes.cpp[m
[1mindex 7b119d2..fa9fa40 100644[m
[1m--- a/Sources/more_shapes.cpp[m
[1m+++ b/Sources/more_shapes.cpp[m
[36m@@ -78,6 +78,36 @@[m [mnamespace oglopp {[m
 		this->updateVAO();[m
 	}[m
 [m
[32m+[m
[32m+[m	[32mCube2::Cube2() {  //   l/r    u/d     f/b\[m
[32m+[m		[32m// Push the data[m
[32m+[m		[32mthis->pushVertex({ 0.5f,  0.5f,  0.5f});	// right up back[m
[32m+[m		[32mthis->pushVertex({ 0.5f,  0.5f, -0.5f}); 	// right up front[m
[32m+[m		[32mthis->pushVertex({ 0.5f, -0.5f,  0.5f}); 	// right down back[m
[32m+[m		[32mthis->pushVertex({ 0.5f, -0.5f, -0.5f});	// right down front[m
[32m+[m		[32mthis->pushVertex({-0.5f,  0.5f,  0.5f});	// left up back[m
[32m+[m		[32mthis->pushVertex({-0.5f,  0.5f, -0.5f});	// left up front[m
[32m+[m		[32mthis->pushVertex({-0.5f, -0.5f,  0.5f});	// left down back[m
[32m+[m		[32mthis->pushVertex({-0.5f, -0.5f, -0.5f});	// left down front[m
[32m+[m
[32m+[m		[32mthis->pushNormal({ 0.0f, 0.0f, 1.0f});		// back[m
[32m+[m		[32mthis->pushNormal({ 0.0f, 1.0f, 0.0f});		// up[m
[32m+[m		[32mthis->pushNormal({ 1.0f, 0.0f, 0.0f});		// right[m
[32m+[m		[32mthis->pushNormal({ 0.0f, 0.0f,-1.0f});		// front[m
[32m+[m		[32mthis->pushNormal({ 0.0f,-1.0f, 0.0f});		// down[m
[32m+[m		[32mthis->pushNormal({-1.0f, 0.0f, 0.0f});		// left[m
[32m+[m
[32m+[m		[32mthis->pushTexCoord({0.0, 0.0});	// bottom left[m
[32m+[m		[32mthis->pushTexCoord({0.0, 1.0});	// top left[m
[32m+[m		[32mthis->pushTexCoord({1.0, 0.0});	// bottom right[m
[32m+[m		[32mthis->pushTexCoord({1.0, 1.0});	// top right[m
[32m+[m
[32m+[m		[32m// Push the indices[m
[32m+[m		[32mthis->pushTriangle();[m
[32m+[m
[32m+[m		[32mthis->updateVAO();[m
[32m+[m	[32m}[m
[32m+[m
 	/** @brief Create a 3D Sphere object.[m
 	 *  @param[in] X_VERTS	The X resolution of the sphere.[m
 	 *  @param[in] Y_VERTS 	The Y resolution of the sphere.[m
[1mdiff --git a/Sources/shape2.cpp b/Sources/shape2.cpp[m
[1mnew file mode 100644[m
[1mindex 0000000..8707bac[m
[1m--- /dev/null[m
[1m+++ b/Sources/shape2.cpp[m
[36m@@ -0,0 +1,597 @@[m
[32m+[m[32m#include <cstdlib>[m
[32m+[m[32m#include <cstdint>[m
[32m+[m[32m#include <glm/ext/vector_float2.hpp>[m
[32m+[m[32m#include <iostream>[m
[32m+[m[32m#include <iterator>[m
[32m+[m
[32m+[m[32m#include "oglopp/defines.h"[m
[32m+[m[32m#include "oglopp/glad/gl.h"[m
[32m+[m[32m#include "oglopp/shader.h"[m
[32m+[m[32m#include "oglopp/shape2.h"[m
[32m+[m
[32m+[m[32m//#define VERTS 18[m
[32m+[m[32m//#define VERT_SIZE (VERTS * sizeof(float))[m
[32m+[m
[32m+[m[32mnamespace oglopp {[m
[32m+[m
[32m+[m	[32m/** @brief Get the opengl texture register for the n'th texture, where index = n[m
[32m+[m	[32m* @param[in]	index	The index/layer of the texture[m
[32m+[m	[32m*/[m
[32m+[m	[32muint16_t Shape2::getTextureCode(uint8_t index) {[m
[32m+[m		[32mreturn static_cast<uint16_t>(GL_TEXTURE0) + index;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Get the opengl texture string for the n'th texture, where index = n[m
[32m+[m	[32m* @param[in]	index	The index/layer of the texture[m
[32m+[m	[32m*/[m
[32m+[m	[32mstd::string Shape2::getTextureString(uint8_t index) {[m
[32m+[m		[32mstd::string str = "texture" + std::to_string(index);[m
[32m+[m
[32m+[m		[32mreturn str;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Update uniform MVP[m
[32m+[m	[32m* @param[in] window	A reference to the window object[m
[32m+[m	[32m* @param[in] pShader	A pointer to the shader object[m
[32m+[m	[32m* @return				A reference to this shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::updateUniformMVP(Window& window, Shader* pShader) {[m
[32m+[m		[32mint width, height;[m
[32m+[m		[32mwindow.getSize(&width, &height);[m
[32m+[m
[32m+[m		[32m// ..:: Model Matrix ::..[m
[32m+[m		[32mglm::dmat4 model(1.f); // Accumulate changes[m
[32m+[m		[32mmodel *= glm::translate(model, this->position); // This is kinda backwards but it gives the correct result[m
[32m+[m		[32mglm::dmat4 rotation(1.f); // Used for transforming normals[m
[32m+[m		[32mif (this->getAngle() != glm::dvec3(0)) {[m
[32m+[m
[32m+[m			[32mrotation = glm::rotate<double>(rotation, this->getAngle().x, glm::dvec3(1.0, 0.0, 0.0f));[m
[32m+[m			[32mrotation = glm::rotate<double>(rotation, this->getAngle().y, glm::dvec3(0.0, 1.0, 0.0f));[m
[32m+[m			[32mrotation = glm::rotate<double>(rotation, this->getAngle().z, glm::dvec3(0.0, 0.0, 1.0f));[m
[32m+[m			[32mmodel *= rotation;[m
[32m+[m		[32m}[m
[32m+[m		[32mmodel = glm::scale(model, this->getScale());[m
[32m+[m
[32m+[m		[32m// ..:: View Matrix ::..[m
[32m+[m		[32mglm::mat4 view(window.getCam().getView()); //window.getCam().face(-window.getCam().getBack());[m
[32m+[m
[32m+[m		[32m// ..:: Projection Matrix ::..[m
[32m+[m		[32mglm::mat4 projection(window.getCam().getProjection());[m
[32m+[m		[32m//projection = //glm::perspective<float>(glm::radians(window.getCam().getFov()), static_cast<float>(width) / static_cast<float>(height), HLGL_RENDER_NEAR, HLGL_RENDER_FAR);[m
[32m+[m
[32m+[m		[32m// ..:: Apply Elements ::..[m
[32m+[m		[32mpShader->use();[m
[32m+[m		[32mpShader->setMat4("model", model);[m
[32m+[m		[32mpShader->setMat4("view", view);[m
[32m+[m		[32mpShader->setMat4("projection", projection);[m
[32m+[m		[32mpShader->setMat4("rotation", rotation);[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Updated extra uniforms. obverloaded in each inherited class[m
[32m+[m	[32m * @return A reference to this shape object[m
[32m+[m[41m [m	[32m*/[m
[32m+[m	[32mShape2& Shape2::updateExtraUniforms() {[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32mShape2& Shape2::updateEBO() {[m
[32m+[m		[32m// Index(element) Buffer Object[m
[32m+[m		[32mglGenBuffers(1, &this->EBO);[m
[32m+[m		[32mglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);[m
[32m+[m		[32mglBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * HLGL_EBO_COMPONENTS * sizeof(Index), this->indices.data(), GL_STATIC_DRAW);[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32mShape2& Shape2::updateVBO() {[m
[32m+[m
[32m+[m		[32m// Construct interleaved buffer out of the components[m
[32m+[m
[32m+[m
[32m+[m
[32m+[m		[32m// Vertex Buffer Object[m
[32m+[m		[32mglGenBuffers(1, &this->VBO);[m
[32m+[m		[32mglBindBuffer(GL_ARRAY_BUFFER, this->VBO);[m
[32m+[m		[32mglBufferData(GL_ARRAY_BUFFER, this->cLists.totalByteSum(), nullptr, GL_STATIC_DRAW);[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/*[m
[32m+[m	[32m * @brief Get the number of components for each vertex, in total. Update strideElements and strideBytes[m
[32m+[m	[32m * @return	The number of components for each vertex.[m
[32m+[m	[32m */[m
[32m+[m	[32minline uint32_t Shape2::getComponentCount() {[m
[32m+[m		[32mthis->strideElements = this->cLists.itemPartSum();[m
[32m+[m		[32mthis->strideBytes = this->cLists.itemByteSum();[m
[32m+[m
[32m+[m		[32mreturn this->strideElements;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Update the vertex, index, and texture coordinate list. Expected to be called when the texture list is modified.[m
[32m+[m	[32m * @param[in] color		Include the color/normal vec3[m
[32m+[m	[32m * @param[in] texture	Include the texture coord vec2[m
[32m+[m	[32m * @param[in] option	Include the option uint16_t[m
[32m+[m	[32m * @return	A reference to this shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::updateVAO() {[m
[32m+[m		[32m// INITIALIZATION - DONE ONCE UNLESS MESH UPDATES[m
[32m+[m		[32m// Calculate the stride bytes[m
[32m+[m		[32mthis->getComponentCount();[m
[32m+[m
[32m+[m		[32mglGenVertexArrays(1, &this->VAO);[m
[32m+[m		[32mglBindVertexArray(this->VAO);[m
[32m+[m
[32m+[m		[32m// Update Vertex Buffer Object[m
[32m+[m		[32mthis->updateVBO();[m
[32m+[m
[32m+[m		[32m// Update Entity Buffer Object[m
[32m+[m		[32mif (!this->indices.empty()) {[m
[32m+[m			[32mthis->updateEBO();[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32muint64_t offset = 0;[m
[32m+[m		[32mint index = 0;[m
[32m+[m
[32m+[m		[32m// Push each component list in order.[m
[32m+[m		[32mfor (IComponentList* cList : this->cLists) {[m
[32m+[m			[32mcList->pushVAOIndex(index, offset);[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32m// Unbind the vertex array[m
[32m+[m		[32mglBindBuffer(GL_ARRAY_BUFFER, 0);[m
[32m+[m		[32mglBindVertexArray(0);[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Replacement for updateVAO. Allows dynamically specifying the type of value. Termination case[m
[32m+[m	[32m * @param[in] totalIndices	The total number of indices[m
[32m+[m	[32m * @return 	A reference to this shape object[m
[32m+[m[41m [m	[32m*/[m
[32m+[m	[32mShape2& Shape2::finalizePoints(const int totalIndices) {[m
[32m+[m		[32mglGenVertexArrays(1, &this->VAO);[m
[32m+[m		[32m// Initialization code (done once (unless your object frequently changes))[m
[32m+[m
[32m+[m		[32m// 1. bind Vertex Array Object[m
[32m+[m		[32mglBindVertexArray(this->VAO);[m
[32m+[m
[32m+[m		[32m// Update Vertex Buffer Object[m
[32m+[m		[32mthis->updateVBO();[m
[32m+[m
[32m+[m		[32m// Update Enitty Buffer Object[m
[32m+[m		[32mif (this->indexCount > 0) {[m
[32m+[m			[32mthis->updateEBO();[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32m//std::cout << "FIN Index count " << this->indexCount << std::endl;[m
[32m+[m		[32m//std::cout << "FIN Stride bytes " << this->strideBytes << std::endl;[m
[32m+[m		[32m//std::cout << "FIN Vert count " << this->vertCount << std::endl;[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Convert a shape DataType to a stride element count[m
[32m+[m	[32m * @param[in] dataType	The data type[m
[32m+[m	[32m * @return				The number of associated stride elements/components[m
[32m+[m[41m [m	[32m*/[m
[32m+[m	[32mconst uint32_t Shape2::getStrideElems(DataType const& dataType) {[m
[32m+[m		[32muint32_t elements = 0;[m
[32m+[m
[32m+[m		[32mswitch(dataType) {[m
[32m+[m			[32mcase FLOAT:[m
[32m+[m			[32mcase DOUBLE:[m
[32m+[m			[32mcase UINT8:[m
[32m+[m			[32mcase UINT16:[m
[32m+[m			[32mcase UINT32:[m
[32m+[m			[32mcase INT8:[m
[32m+[m			[32mcase INT16:[m
[32m+[m			[32mcase INT32:[m
[32m+[m				[32melements = 1;[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase VEC2:[m
[32m+[m			[32mcase DVEC2:[m
[32m+[m			[32mcase IVEC2:[m
[32m+[m			[32mcase I64VEC2:[m
[32m+[m			[32mcase UVEC2:[m
[32m+[m			[32mcase U64VEC2:[m
[32m+[m				[32melements = 2;[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase VEC3:[m
[32m+[m			[32mcase DVEC3:[m
[32m+[m			[32mcase IVEC3:[m
[32m+[m			[32mcase I64VEC3:[m
[32m+[m			[32mcase UVEC3:[m
[32m+[m			[32mcase U64VEC3:[m
[32m+[m				[32melements = 3;[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase VEC4:[m
[32m+[m			[32mcase DVEC4:[m
[32m+[m			[32mcase IVEC4:[m
[32m+[m			[32mcase I64VEC4:[m
[32m+[m			[32mcase UVEC4:[m
[32m+[m			[32mcase U64VEC4:[m
[32m+[m				[32melements = 4;[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32m// No default case. Specify all types. Let compiler warn us otherwise.[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32mreturn elements;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Get the number of bytes for a single stride component for some datatype[m
[32m+[m	[32m * @param[in] dataType	The data type[m
[32m+[m	[32m * @return				The number of associated stride elements/components[m
[32m+[m[41m [m	[32m*/[m
[32m+[m	[32mconst uint32_t Shape2::getStrideComponentBytes(DataType const& dataType) {[m
[32m+[m		[32muint32_t size = 0;[m
[32m+[m
[32m+[m		[32mswitch(dataType) {[m
[32m+[m			[32mcase FLOAT:[m
[32m+[m			[32mcase VEC2:[m
[32m+[m			[32mcase VEC3:[m
[32m+[m			[32mcase VEC4:[m
[32m+[m				[32msize = sizeof(float);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase DOUBLE:[m
[32m+[m			[32mcase DVEC2:[m
[32m+[m			[32mcase DVEC3:[m
[32m+[m			[32mcase DVEC4:[m
[32m+[m				[32msize = sizeof(double);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase UINT8:[m
[32m+[m				[32msize = sizeof(uint8_t);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase UINT16:[m
[32m+[m				[32msize = sizeof(uint16_t);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase UINT32:[m
[32m+[m			[32mcase UVEC2:[m
[32m+[m			[32mcase UVEC3:[m
[32m+[m			[32mcase UVEC4:[m
[32m+[m				[32msize = sizeof(uint32_t);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase INT8:[m
[32m+[m				[32msize = sizeof(int8_t);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase INT16:[m
[32m+[m				[32msize = sizeof(int16_t);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase INT32:[m
[32m+[m			[32mcase IVEC2:[m
[32m+[m			[32mcase IVEC3:[m
[32m+[m			[32mcase IVEC4:[m
[32m+[m				[32msize = sizeof(int32_t);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase I64VEC2:[m
[32m+[m			[32mcase I64VEC3:[m
[32m+[m			[32mcase I64VEC4:[m
[32m+[m				[32msize = sizeof(int64_t);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase U64VEC2:[m
[32m+[m			[32mcase U64VEC3:[m
[32m+[m			[32mcase U64VEC4:[m
[32m+[m				[32msize = sizeof(uint64_t);[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32m// No default case. Specify all types. Let compiler warn us otherwise.[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32mreturn size;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Get the component register data type for some shape dataype[m
[32m+[m	[32m * @param[in] dataType	The shape datatype[m
[32m+[m	[32m * @return				The gl register[m
[32m+[m[41m [m	[32m*/[m
[32m+[m[41m  [m	[32mconst uint32_t Shape2::getStructComponentRegister(DataType const& dataType) {[m
[32m+[m[41m   [m		[32muint32_t reg = 0;[m
[32m+[m
[32m+[m		[32mswitch(dataType) {[m
[32m+[m			[32mcase FLOAT:[m
[32m+[m			[32mcase VEC2:[m
[32m+[m			[32mcase VEC3:[m
[32m+[m			[32mcase VEC4:[m
[32m+[m				[32mreg = GL_FLOAT;[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase DOUBLE:[m
[32m+[m			[32mcase DVEC2:[m
[32m+[m			[32mcase DVEC3:[m
[32m+[m			[32mcase DVEC4:[m
[32m+[m				[32mreg = GL_DOUBLE;[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32mcase UINT8:[m
[32m+[m			[32mcase UINT16:[m
[32m+[m			[32mcase UINT32:[m
[32m+[m			[32mcase UVEC2:[m
[32m+[m			[32mcase UVEC3:[m
[32m+[m			[32mcase UVEC4:[m
[32m+[m			[32mcase INT8:[m
[32m+[m			[32mcase INT16:[m
[32m+[m			[32mcase INT32:[m
[32m+[m			[32mcase IVEC2:[m
[32m+[m			[32mcase IVEC3:[m
[32m+[m			[32mcase IVEC4:[m
[32m+[m			[32mcase I64VEC2:[m
[32m+[m			[32mcase I64VEC3:[m
[32m+[m			[32mcase I64VEC4:[m
[32m+[m			[32mcase U64VEC2:[m
[32m+[m			[32mcase U64VEC3:[m
[32m+[m			[32mcase U64VEC4:[m
[32m+[m				[32mreg = dataType;[m
[32m+[m				[32mbreak;[m
[32m+[m
[32m+[m			[32m// No default case. Specify all types. Let compiler warn us otherwise.[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32mreturn reg;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32mShape2::Shape2() {[m
[32m+[m		[32m// Initialize stuff[m
[32m+[m		[32m//std::memset(this->textures, 0, sizeof(Texture*) * HLGL_SHAPE_MAX_TEXTURES);[m
[32m+[m		[32mthis->position = glm::vec3(0, 0, 0);[m
[32m+[m		[32mthis->angle = glm::vec3(0, 0, 0);[m
[32m+[m		[32mthis->scaleVec = glm::vec3(1, 1 ,1);[m
[32m+[m		[32mthis->size = 0;[m
[32m+[m		[32mthis->myRegister = 0;[m
[32m+[m		[32mthis->strideElements = 0;[m
[32m+[m		[32mthis->strideBytes = 0;[m
[32m+[m
[32m+[m		[32mthis->cLists.push_back(&this->cVertices);[m
[32m+[m		[32mthis->cLists.push_back(&this->cNormals);[m
[32m+[m		[32mthis->cLists.push_back(&this->cTexCoords);[m
[32m+[m		[32mthis->cLists.push_back(&this->cNormals);[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32mShape2::~Shape2() {[m
[32m+[m		[32mglDeleteBuffers(1, &this->VBO);[m
[32m+[m		[32mglDeleteBuffers(1, &this->EBO);[m
[32m+[m		[32mglDeleteVertexArrays(1, &this->VAO);[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/**[m
[32m+[m	[32m * @brief Push a single vertex to the shape[m
[32m+[m	[32m * @param[in] vector	The vector of the vertex[m
[32m+[m	[32m * @return 				A reference to this shape object[m
[32m+[m	[32m */[m
[32m+[m	[32mShape2& Shape2::pushVertex(glm::vec3 vector) {[m
[32m+[m		[32mthis->cVertices.push_back({.x=vector.x, .y=vector.y, .z=vector.z});[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/**[m
[32m+[m	[32m * @brief Push a single normal vector to the shape[m
[32m+[m	[32m * @param[in] normal	The normal vector to push[m
[32m+[m	[32m * @return 				A reference to this shape object[m
[32m+[m	[32m */[m
[32m+[m	[32mShape2& Shape2::pushNormal(glm::vec3 normal) {[m
[32m+[m		[32mthis->cNormals.push_back({.x=normal.x, .y=normal.y, .z=normal.z});[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/**[m
[32m+[m	[32m * @brief Push a single texture coordinate to this shape[m
[32m+[m	[32m * @param[in] texCoord	The texture coordinate to push[m
[32m+[m	[32m * @return 				A reference to this shape object[m
[32m+[m	[32m */[m
[32m+[m	[32mShape2& Shape2::pushTexCoord(glm::vec2 texCoord) {[m
[32m+[m		[32mthis->cTexCoords.push_back({.x=texCoord.x, .y=texCoord.y});[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/**[m
[32m+[m	[32m * @brief Push a single option to this shape[m
[32m+[m	[32m * @param[in] option	The option to push[m
[32m+[m	[32m * @return 				A reference to this shape object[m
[32m+[m	[32m */[m
[32m+[m	[32mShape2& Shape2::pushOption(uint16_t option) {[m
[32m+[m		[32mthis->cOptions.push_back({.opt=option});[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m
[32m+[m
[32m+[m	[32m/** @brief Push a triangle to the indicies list. A triangle is constructed of the following verticies which were defined with pushPoint[m
[32m+[m	[32m * @param[in] vertA	The A vertex index out of the point list, where the first point is 0[m
[32m+[m	[32m * @param[in] vertB	The B vertex index[m
[32m+[m	[32m * @param[in] vertC	The C vertex index[m
[32m+[m	[32m * @return			A reference to this shape object[m
[32m+[m	[32m */[m
[32m+[m	[32mShape2& Shape2::pushTriangle(unsigned int vertA, unsigned int vertB, unsigned int vertC) {[m
[32m+[m		[32mthis->indices.push_back(vertA);[m
[32m+[m		[32mthis->indices.push_back(vertB);[m
[32m+[m		[32mthis->indices.push_back(vertC);[m
[32m+[m
[32m+[m		[32mthis->indexCount++; // One index represents 3 points[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Set the texture[m
[32m+[m	[32m* @param[in] texture	The texture object to set to[m
[32m+[m	[32m* @return				A reference to this shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::pushTexture(Texture* newTexture) {[m
[32m+[m		[32mif (nullptr != newTexture) {[m
[32m+[m			[32mthis->textures.push_back(newTexture);[m
[32m+[m		[32m}[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32munsigned int Shape2::getVAO() {[m
[32m+[m		[32mreturn this->VAO;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32munsigned int Shape2::getVBO() {[m
[32m+[m		[32mreturn this->VBO;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32mstd::vector<Texture*>& Shape2::getTextureList() {[m
[32m+[m		[32mreturn this->textures;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Draw this shape to the specified window using an optional shader[m
[32m+[m	[32m* @param[in] window		A reference to the window object[m
[32m+[m	[32m* @param[in] pShader	An optional pointer to the shader object[m
[32m+[m	[32m* @return 				A reference to this shape[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::draw(Window& window, Shader* pShader) {[m
[32m+[m		[32mDrawType drawType = TRIANGLES;[m
[32m+[m
[32m+[m		[32mthis->size = this->textures.size();[m
[32m+[m
[32m+[m		[32m// Do this stuff if the shader was specified[m
[32m+[m		[32mif (pShader != nullptr) {[m
[32m+[m			[32mdrawType = pShader->getDrawType();[m
[32m+[m
[32m+[m			[32m// Use the shader for the following actions in this block...[m
[32m+[m			[32mpShader->use();[m
[32m+[m
[32m+[m			[32m// Perform the model - view - projection calculation and pass through to the shader[m
[32m+[m			[32mthis->updateUniformMVP(window, pShader);[m
[32m+[m
[32m+[m			[32m// Apply textures to this object using the shader[m
[32m+[m			[32mfor (int16_t i=0;i<this->size;i++) {[m
[32m+[m				[32mmyRegister = Shape2::getTextureCode(i);[m
[32m+[m
[32m+[m				[32m// Bind the texture. This is required to be performed for each frame, for each texture, for each object.[m
[32m+[m				[32mthis->textures[i]->bind(myRegister);[m
[32m+[m
[32m+[m				[32m// We want to re-set the texture string every frame in case the shader changes[m
[32m+[m				[32m// There are much better ways to do this but it's just one line soo like...[m
[32m+[m				[32mpShader->setInt(Shape2::getTextureString(i), i);[m
[32m+[m			[32m}[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32m// Bind vertex array[m
[32m+[m		[32mglBindVertexArray(this->VAO);[m
[32m+[m
[32m+[m		[32m// Draw[m
[32m+[m		[32mswitch (drawType) {[m
[32m+[m			[32mdefault:[m
[32m+[m			[32mcase TRIANGLES: {[m
[32m+[m				[32mif (this->indexCount > 0) {[m
[32m+[m					[32mglDrawElements(GL_TRIANGLES, this->strideElements, GL_UNSIGNED_INT, 0);[m
[32m+[m				[32m} else {[m
[32m+[m					[32mglDrawArrays(GL_TRIANGLES, 0, this->cVertices.size());[m
[32m+[m				[32m}[m
[32m+[m				[32mbreak;[m
[32m+[m			[32m}[m
[32m+[m
[32m+[m			[32mcase LINE_LOOP:[m
[32m+[m			[32mcase LINE: {[m
[32m+[m				[32mif (this->indexCount > 0) {[m
[32m+[m					[32mglDrawElements(GL_LINES, this->strideElements, GL_UNSIGNED_INT, 0);[m
[32m+[m			[41m [m	[32m} else {[m
[32m+[m			[41m [m		[32mglDrawArrays((drawType == LINE) ? GL_LINE_STRIP : GL_LINE_LOOP, 0, this->cVertices.size());[m
[32m+[m			[41m [m	[32m}[m
[32m+[m			[41m [m	[32mbreak;[m
[32m+[m			[32m }[m
[32m+[m
[32m+[m			[32mcase POINTS: {[m
[32m+[m				[32mglDrawArrays(GL_POINTS, 0, this->cVertices.size());[m
[32m+[m				[32mbreak;[m
[32m+[m			[32m}[m
[32m+[m		[32m}[m
[32m+[m
[32m+[m		[32m// Unbind vertex array[m
[32m+[m		[32mglBindVertexArray(0);[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Get the position of this shape[m
[32m+[m	[32m* @return The position of this shape[m
[32m+[m	[32m*/[m
[32m+[m	[32mglm::dvec3 const& Shape2::getPosition() {[m
[32m+[m		[32mreturn this->position;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Get the angle of this shape[m
[32m+[m	[32m* @return The angle of this shape[m
[32m+[m	[32m*/[m
[32m+[m	[32mglm::dvec3 const& Shape2::getAngle() {[m
[32m+[m		[32mreturn this->angle;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Set the position of this shape in world space[m
[32m+[m	[32m* @param[in] newPosition	The position in world space[m
[32m+[m	[32m* @return					A reference to this position[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::setPosition(glm::dvec3 newPosition) {[m
[32m+[m		[32mthis->position = newPosition;[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Set the angle of this shape in radians for each axis[m
[32m+[m	[32m* @param[in] newAngle		The angle to set to[m
[32m+[m	[32m* @return 					A reference to this shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::setAngle(glm::dvec3 newAngle) {[m
[32m+[m		[32mthis->angle = newAngle;[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Translate this shape some coordinates in world space[m
[32m+[m	[32m* @param[in] offset	The offset to translate by[m
[32m+[m	[32m* @return				A reference to this shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::translate(glm::dvec3 offset) {[m
[32m+[m		[32mthis->position += offset;[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Rotate this shape around its local origin[m
[32m+[m	[32m* @param[in] offset	The offset to rotate by in radians for each axis[m
[32m+[m	[32m* @return				A reference to this shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::rotate(glm::dvec3 offset) {[m
[32m+[m		[32mthis->angle += offset;[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Set the scale of the shape[m
[32m+[m	[32m* @param[in] newScale	The new scale for this shape[m
[32m+[m	[32m* @return 				A reference to this shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::setScale(glm::dvec3 newScale) {[m
[32m+[m		[32mthis->scaleVec = newScale;[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Apply a scaling factor to the shape[m
[32m+[m	[32m* @param[in] offset	The new scale for this shape[m
[32m+[m	[32m* @return				A reference to this shape object[m
[32m+[m	[32m*/[m
[32m+[m	[32mShape2& Shape2::scale(glm::dvec3 offset) {[m
[32m+[m		[32mthis->scaleVec *= offset;[m
[32m+[m
[32m+[m		[32mreturn *this;[m
[32m+[m	[32m}[m
[32m+[m
[32m+[m	[32m/** @brief Get the scale factor[m
[32m+[m	[32m* @return The scaling factor[m
[32m+[m	[32m*/[m
[32m+[m	[32mglm::dvec3 const& Shape2::getScale() {[m
[32m+[m		[32mreturn this->scaleVec;[m
[32m+[m	[32m}[m
[32m+[m[32m}[m
