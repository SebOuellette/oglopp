#include "../Headers/oglopp.h"
#include <cstdlib>

//#define VERTS 18
//#define VERT_SIZE (VERTS * sizeof(float))

namespace oglopp {
	Shape::Shape() {
		// float tmpVerts[] = {
		// 	 0.5, -0.5,  0.0,	1.0, 0.0, 0.0, // Bottom left
		// 	-0.5, -0.5,  0.0,	0.0, 1.0, 0.0, // Bottom right
		// 	 0.0,  0.5,  0.0,	0.0, 0.0, 1.0  // Top
		// };

		// this->vertices = new float[VERTS] {
		// 	 0.5, -0.5,  0.0,	1.0, 0.0, 0.0, // Bottom left
		// 	-0.5, -0.5,  0.0,	0.0, 1.0, 0.0, // Bottom right
		// 	 0.0,  0.5,  0.0,	0.0, 0.0, 1.0  // Top
		// };

		//std::memcpy(this->vertices, tmpVerts, VERT_SIZE);

		// Vertex buffer obejct


		// ----- Vertex Array Object -----
		//updateVAO();
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
	Shape& Shape::setTexture(Texture* newTexture) {
		this->texture = newTexture;


		// Change to push texture instead of set texture

		return *this;
	}
	

	Shape& Shape::updateEBO() {
		// Create the element buffer object
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexCount * HLGL_STRIDE_ELEMENTS * sizeof(int), this->indices.data(), GL_STATIC_DRAW);

		return *this;
	}

	Shape& Shape::updateVAO() {
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

		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(HLGL_VEC_INDEX, HLGL_VEC_COMPONENTS, GL_FLOAT, GL_FALSE, HLGL_STRIDE_BYTES, (void*)0);
		glEnableVertexAttribArray(HLGL_VEC_INDEX);

		// 4. Set the colour attribute
		glVertexAttribPointer(HLGL_COL_INDEX, HLGL_COL_COMPONENTS, GL_FLOAT, GL_FALSE, HLGL_STRIDE_BYTES, (void*)(HLGL_VEC_COMPONENTS * sizeof(float)));
		glEnableVertexAttribArray(HLGL_COL_INDEX);

		// 5. Set the texture attribute
		glVertexAttribPointer(HLGL_TEX_INDEX, HLGL_TEX_COMPONENTS, GL_FLOAT, GL_FALSE, HLGL_STRIDE_BYTES, (void*)((HLGL_VEC_COMPONENTS + HLGL_COL_COMPONENTS) * sizeof(float)));
		glEnableVertexAttribArray(HLGL_TEX_INDEX);

		// Unbind the vertex array
		glBindVertexArray(0);

		return *this;
	}

	Shape& Shape::updateVBO() {
		// Create an empty vertex buffer object
		glGenBuffers(1, &this->VBO);

		// Bind the newly created buffer to the array buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		// Copy the vertex array data into the buffer
		glBufferData(GL_ARRAY_BUFFER, this->vertCount * HLGL_STRIDE_BYTES, this->vertices.data(), GL_STATIC_DRAW);

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

	void Shape::draw() {
		if (this->texture != nullptr) {
			glBindTexture(GL_TEXTURE_2D, this->texture->getTexture());
		}

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, HLGL_STRIDE_ELEMENTS, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
