#include <cstdlib>
#include <cstdint>
#include <iostream>

#include "oglopp/defines.h"
#include "oglopp/glad/gl.h"
#include "oglopp/shader.h"
#include "oglopp/shape.h"

//#define VERTS 18
//#define VERT_SIZE (VERTS * sizeof(float))

namespace oglopp {

	/* @brief Get the opengl texture register for the n'th texture, where index = n
	* @param[in]	index	The index/layer of the texture
	*/
	uint16_t Shape::getTextureCode(uint8_t index) {
		return static_cast<uint16_t>(GL_TEXTURE0) + index;
	}

	/* @brief Get the opengl texture string for the n'th texture, where index = n
	* @param[in]	index	The index/layer of the texture
	*/
	std::string Shape::getTextureString(uint8_t index) {
		std::string str = "texture" + std::to_string(index);

		return str;
	}

	/* @brief Update uniform MVP
	* @param[in] window	A reference to the window object
	* @param[in] pShader	A pointer to the shader object
	* @return				A reference to this shape object
	*/
	Shape& Shape::updateUniformMVP(Window& window, Shader* pShader) {
		int width, height;
		window.getSize(&width, &height);

		// ..:: Model Matrix ::..
		glm::mat4 model(1.f); // Accumulate changes
		model = glm::scale(model, this->getScale());
		model *= glm::translate(model, this->position); // This is kinda backwards but it gives the correct result
		glm::mat4 rotation(1.f); // Used for transforming normals
		if (this->getAngle() != glm::vec3(0)) {

			rotation = glm::rotate<float>(rotation, this->getAngle().x, glm::vec3(1.0, 0.0, 0.0f));
			rotation = glm::rotate<float>(rotation, this->getAngle().y, glm::vec3(0.0, 1.0, 0.0f));
			rotation = glm::rotate<float>(rotation, this->getAngle().z, glm::vec3(0.0, 0.0, 1.0f));
			model *= rotation;
		}

		// ..:: View Matrix ::..
		glm::mat4 view(window.getCam().getView()); //window.getCam().face(-window.getCam().getBack());

		//glm::mat4 view(1.0f);
		//view = glm::translate(view, -window.getCam().getPos());
		//view = glm::rotate(view, glm::radians(45.f), glm::vec3(1.0, 0.0, 0.0));
		//view = glm::rotate(view, glm::radians(0.f), glm::vec3(0.0, 1.0, 0.0));


		// ..:: Projection Matrix ::..
		glm::mat4 projection(window.getCam().getProjection());
		//projection = //glm::perspective<float>(glm::radians(window.getCam().getFov()), static_cast<float>(width) / static_cast<float>(height), HLGL_RENDER_NEAR, HLGL_RENDER_FAR);

		// ..:: Apply Elements ::..
		pShader->use();
		pShader->setMat4("model", model);
		pShader->setMat4("view", view);
		pShader->setMat4("projection", projection);
		pShader->setMat4("rotation", rotation);

		return *this;
	}

	/* @brief Updated extra uniforms. obverloaded in each inherited class
	 * @return A reference to this shape object
 	*/
	Shape& Shape::updateExtraUniforms() {
		return *this;
	}

	Shape& Shape::updateEBO() {
		// Create the element buffer object
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexCount * this->strideElements * sizeof(int), this->indices.data(), GL_STATIC_DRAW);

		return *this;
	}

	Shape& Shape::updateVBO() {
		// Create an empty vertex buffer object
		glGenBuffers(1, &this->VBO);

		// Bind the newly created buffer to the array buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		// Copy the vertex array data into the buffer
		glBufferData(GL_ARRAY_BUFFER, this->vertCount * this->strideBytes/*this->strideElements * sizeof(float)*/, this->vertices.data(), GL_STATIC_DRAW);

		return *this;
	}

	/* @brief Update the vertex, index, and texture coordinate list. Expected to be called when the texture list is modified.
	 * @param[in] color		Include the color/normal vec3
	 * @param[in] texture	Include the texture coord vec2
	 * @param[in] option	Include the option uint16_t
	 * @return	A reference to this shape object
	*/
	Shape& Shape::updateVAO(bool color, bool texture, bool option) {
		// Calculate the stride bytes
		this->strideElements = HLGL_VEC_COMPONENTS + (color ? HLGL_COL_COMPONENTS : 0) + (texture ? HLGL_TEX_COMPONENTS : 0) + (option ? HLGL_OPT_COMPONENTS : 0);
		this->strideBytes = this->strideElements * sizeof(float);
			//HLGL_VEC_COMPONENTS * sizeof(float) + (color ? HLGL_COL_COMPONENTS * sizeof(float) : 0) + (texture ? HLGL_TEX_COMPONENTS * sizeof(float) : 0) + (option ? HLGL_OPT_COMPONENTS * sizeof(float) : 0);

		glGenVertexArrays(1, &this->VAO);
		// Initialization code (done once (unless your object frequently changes))

		// 1. bind Vertex Array Object
		glBindVertexArray(this->VAO);

		// Update Vertex Buffer Object
		this->updateVBO();

		// Update Enitty Buffer Object
		if (this->indexCount > 0) {
			this->updateEBO();
		}

		unsigned long int offset = 0;
		int index = 0;

		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(index, HLGL_VEC_COMPONENTS, GL_FLOAT, GL_FALSE, this->strideBytes, (void*)offset);
		glEnableVertexAttribArray(index);
		offset += HLGL_VEC_COMPONENTS * sizeof(float);
		index++;


		// Color (Used for Normals now uhhh idk man how this stuff is supposed to be generalized now.. I Need like a billion templates and stuff I don't wanna)
		if (color) {
			// 4. Set the colour attribute
			glVertexAttribPointer(index, HLGL_COL_COMPONENTS, GL_FLOAT, GL_FALSE, this->strideBytes, (void*)offset);
			glEnableVertexAttribArray(index);
			offset += HLGL_COL_COMPONENTS * sizeof(float);
		}
		index++;

		if (texture) {
			// 5. Set the texture attribute
			glVertexAttribPointer(index, HLGL_TEX_COMPONENTS, GL_FLOAT, GL_FALSE, this->strideBytes, (void*)offset);
			glEnableVertexAttribArray(index);
			offset += HLGL_TEX_COMPONENTS * sizeof(float);
		}
		index++;

		if (option) {
			// 5. Set the texture attribute
			glVertexAttribPointer(index, HLGL_OPT_COMPONENTS, GL_FLOAT, GL_FALSE, this->strideBytes, (void*)offset);
			glEnableVertexAttribArray(index);
			offset += HLGL_OPT_COMPONENTS * sizeof(float);
		}
		index++;

		// Unbind the vertex array
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return *this;
	}

	Shape::Shape() {
		// Initialize stuff
		//std::memset(this->textures, 0, sizeof(Texture*) * HLGL_SHAPE_MAX_TEXTURES);
		this->position = glm::vec3(0, 0, 0);
		this->angle = glm::vec3(0, 0, 0);
		this->scaleVec = glm::vec3(1, 1 ,1);
		this->size = 0;
		this->myRegister = 0;
		this->strideElements = 0;
	}

	Shape::~Shape() {
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
		glDeleteVertexArrays(1, &this->VAO);
	}

	/* @brief Push a single point to the shape.
	* @param[in]	vec		The vector of the point
	* @param[in]	col		The color of the vertex
	* @param[in]	texPos	The texture position
	* @param[in]	option	The option to push
	* @return 		A reference to this shape object
	*/
	Shape& Shape::pushPoint(glm::vec3 vec, glm::vec3 col, glm::vec2 texPos, float option) {
		this->pushPoint(vec, col, texPos); // This does the vertCount++
		this->vertices.push_back(option);

		return *this;
	}

	/* @brief Push a single point to the shape.
	* @param[in]	vec		The vector of the point
	* @param[in]	col		The color of the vertex
	* @param[in]	texPos	The texture position
	* @return 		A reference to this shape object
	*/
	Shape& Shape::pushPoint(glm::vec3 vec, glm::vec3 col, glm::vec2 texPos) {
		// Push vertex
		this->vertices.push_back(vec.x);
		this->vertices.push_back(vec.y);
		this->vertices.push_back(vec.z);

		// Push color
		this->vertices.push_back(col.x);
		this->vertices.push_back(col.y);
		this->vertices.push_back(col.z);

		// Push texture coord
		this->vertices.push_back(texPos.x);
		this->vertices.push_back(texPos.y);

		this->vertCount++;
		//this->strideElements = HLGL_VEC_COMPONENTS + HLGL_COL_COMPONENTS + HLGL_TEX_COMPONENTS;

		return *this;
	}

	/* @brief Push a single point to the shape.
	* @param[in]	vec		The vector of the point
	* @param[in]	col		The color of the vertex
	* @return 		A reference to this shape object
	*/
	Shape& Shape::pushPoint(glm::vec3 vec, glm::vec3 col) {
		this->vertices.push_back(vec.x);
		this->vertices.push_back(vec.y);
		this->vertices.push_back(vec.z);

		this->vertices.push_back(col.x);
		this->vertices.push_back(col.y);
		this->vertices.push_back(col.z);

		this->vertCount++;
		//this->strideElements = HLGL_VEC_COMPONENTS + HLGL_COL_COMPONENTS + HLGL_TEX_COMPONENTS;

		return *this;
	}

	/* @brief Push a single point to the shape.
	* @param[in]	vec		The vector of the point
	* @param[in]	texPos	The texture position
	* @return 		A reference to this shape object
	*/
	Shape& Shape::pushPoint(glm::vec3 vec, glm::vec2 texPos) {
		this->vertices.push_back(vec.x);
		this->vertices.push_back(vec.y);
		this->vertices.push_back(vec.z);

		this->vertices.push_back(texPos.x);
		this->vertices.push_back(texPos.y);

		this->vertCount++;

		return *this;
	}

	/* @brief Push a single point to the shape.
	* @param[in]	vec		The vector of the point
	* @return 		A reference to this shape object
	*/
	Shape& Shape::pushPoint(glm::vec3 vec) {
		this->vertices.push_back(vec.x);
		this->vertices.push_back(vec.y);
		this->vertices.push_back(vec.z);

		this->vertCount++;

		return *this;
	}

	/* @brief Push a triangle to the indicies list. A triangle is constructed of the following verticies which were defined with pushPoint
	 * @param[in] vertA	The A vertex index out of the point list, where the first point is 0
	 * @param[in] vertB	The B vertex index
	 * @param[in] vertC	The C vertex index
	 * @return			A reference to this shape object
	 */
	Shape& Shape::pushTriangle(unsigned int vertA, unsigned int vertB, unsigned int vertC) {
		this->indices.push_back(vertA);
		this->indices.push_back(vertB);
		this->indices.push_back(vertC);

		this->indexCount++;

		return *this;
	}

	/* @brief Set the texture
	* @param[in] texture	The texture object to set to
	* @return				A reference to this shape object
	*/
	Shape& Shape::pushTexture(Texture const& newTexture) {
		this->textures.push_back(newTexture);

		return *this;
	}

	unsigned int Shape::getVAO() {
		return this->VAO;
	}

	unsigned int Shape::getVBO() {
		return this->VBO;
	}

	std::vector<float>& Shape::getVertices() {
		return this->vertices;
	}

	std::vector<Texture>& Shape::getTextureList() {
		return this->textures;
	}

	/* @brief Draw this shape to the specified window using an optional shader
	* @param[in] window		A reference to the window object
	* @param[in] pShader	An optional pointer to the shader object
	* @return 				A reference to this shape
	*/
	Shape& Shape::draw(Window& window, Shader* pShader) {
		DrawType drawType = TRIANGLES;

		if (pShader != nullptr) {
			drawType = pShader->getDrawType();

			pShader->use();

			// Perform the model - view - projection calculation and pass through to the shader
			this->updateUniformMVP(window, pShader);
		}

		this->size = this->textures.size();

		//while (size > HLGL_SHAPE_MAX_TEXTURES) { // What the hell? Somebody is messing with me...
		//	std::cerr << "Popping extra texture. Did you try to stuff the texture buffer? Do you really need more than " << HLGL_SHAPE_MAX_TEXTURES << "?" << std::endl;
		//	this->textures.pop_back(); // Cant fool me...
		//}

		if (pShader != nullptr) {
			for (int16_t i=0;i<this->size;i++) {
				myRegister = Shape::getTextureCode(i);

				this->textures[i].bind(myRegister);
				pShader->setInt(Shape::getTextureString(i), i);
			}
		}

		// Bind vertex array
		glBindVertexArray(this->VAO);

		// Draw
		switch (drawType) {
			default:
			case TRIANGLES: {
				if (this->indexCount > 0) {
					glDrawElements(GL_TRIANGLES, this->strideElements, GL_UNSIGNED_INT, 0);
				} else {
					glDrawArrays(GL_TRIANGLES, 0, this->vertCount);
				}
				break;
			}

			case POINTS: {
				glDrawArrays(GL_POINTS, 0, this->vertCount);
			}
		}

		// Unbind vertex array
		glBindVertexArray(0);

		/*
		for (uint i=0;i<this->size;i++) {
			myRegister = Shape::getTextureCode(i);

			glActiveTexture(myRegister);
			glBindTexture(GL_TEXTURE_2D, 0);

			if (pShader != nullptr) {
				pShader->setInt(Shape::getTextureString(i), 0);
			}
		}*/

		return *this;
	}

	/* @brief Get the position of this shape
	* @return The position of this shape
	*/
	glm::vec3 const& Shape::getPosition() {
		return this->position;
	}

	/* @brief Get the angle of this shape
	* @return The angle of this shape
	*/
	glm::vec3 const& Shape::getAngle() {
		return this->angle;
	}

	/* @brief Set the position of this shape in world space
	* @param[in] newPosition	The position in world space
	* @return					A reference to this position
	*/
	Shape& Shape::setPosition(glm::vec3 newPosition) {
		this->position = newPosition;
		return *this;
	}

	/* @brief Set the angle of this shape in radians for each axis
	* @param[in] newAngle		The angle to set to
	* @return 					A reference to this shape object
	*/
	Shape& Shape::setAngle(glm::vec3 newAngle) {
		this->angle = newAngle;
		return *this;
	}

	/* @brief Translate this shape some coordinates in world space
	* @param[in] offset	The offset to translate by
	* @return				A reference to this shape object
	*/
	Shape& Shape::translate(glm::vec3 offset) {
		this->position += offset;
		return *this;
	}

	/* @brief Rotate this shape around its local origin
	* @param[in] offset	The offset to rotate by in radians for each axis
	* @return				A reference to this shape object
	*/
	Shape& Shape::rotate(glm::vec3 offset) {
		this->angle += offset;
		return *this;
	}

	/* @brief Set the scale of the shape
	* @param[in] newScale	The new scale for this shape
	* @return 				A reference to this shape object
	*/
	Shape& Shape::setScale(glm::vec3 newScale) {
		this->scaleVec = newScale;

		return *this;
	}

	/* @brief Apply a scaling factor to the shape
	* @param[in] offset	The new scale for this shape
	* @return				A reference to this shape object
	*/
	Shape& Shape::scale(glm::vec3 offset) {
		this->scaleVec *= offset;

		return *this;
	}

	/* @brief Get the scale factor
	* @return The scaling factor
	*/
	glm::vec3 const& Shape::getScale() {
		return this->scaleVec;
	}
}
