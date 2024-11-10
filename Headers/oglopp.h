#ifndef HL_OGLOPP_H
#define HL_OGLOPP_H

// Includes
#include <cstdlib>
#include <cstring>
#include <glm/ext/vector_float2_precision.hpp>
#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Helpful defines
#define HLGL_WORLD_UP 		glm::vec3(0.0f, 1.0f, 0.0f)

#define HLGL_VEC_INDEX			0
#define HLGL_COL_INDEX			1
#define HLGL_TEX_INDEX			2

#define HLGL_VEC_COMPONENTS 	3
#define HLGL_COL_COMPONENTS 	3
#define HLGL_TEX_COMPONENTS		2

#define HLGL_STRIDE_ELEMENTS 	(HLGL_VEC_COMPONENTS + HLGL_COL_COMPONENTS + HLGL_TEX_COMPONENTS)
#define HLGL_STRIDE_BYTES		(HLGL_STRIDE_ELEMENTS * sizeof(float))

namespace oglopp {

	/* @brief Hold a single instance of an object to safely initialize and destroy opengl resources
	*/
	class _HoneyLib_InitGL {
	public:
		_HoneyLib_InitGL();
		~_HoneyLib_InitGL();
	};

	extern _HoneyLib_InitGL _instance;

	std::ostream& operator<<(std::ostream& os, glm::mat4 const& obj);
	std::ostream& operator<<(std::ostream& os, glm::vec4 const& obj);
	std::ostream& operator<<(std::ostream& os, glm::vec3 const& obj);

	/* @brief Utilities for matrix math. Translate, scale, and rotate coordinates. 
	*/
	class Matrix {
	public:
		/* @brief Print an N by M matrix
		 * @param[in]	matrix	The matrix object, where elements are expected to be indexed using [column][row]
		 * @param[in]	cols	The number of columns in the matrix
		 * @param[in]	rows	The number of rows in the matrix
		 * @return				A reference to a stringstream object
		*/
		template<class T>
		static std::ostream& printMatrix(std::ostream& os, T matrix, short int cols, short int rows);

		/* @brief Print an N dimension vector
		 * @param[in]	vector	The matrix object, where elements are expected to be indexed using [row]
		 * @param[in]	rows	The number of rows in the matrix
		 * @return				A reference to a stringstream object
		*/
		template<class T>
		static std::ostream& printVector(std::ostream& os, T matrix, short int rows);

		/* @brief Translate some coordinate by an offset
		 * @param[in]	coordinate	The original input coordinate
		 * @param[in]	offset		The offset to adjust coordinate by
		 * @return					The translated vector. For optimization purposes, this simply adds coordinate and offset instead of a matrix multiplication
		*/
		static glm::vec3 translate(glm::vec3 const& coordinate, glm::vec3 const& offset);

		/* @brief Rotate some coordinate around a provided origin
		 * @param[in]	coordinate	The original input coordinate
		 * @param[in]	theta		The angle to rotate by, in radians
		 * @param[in]	axis		The axis of rotation
		 * @param[in]	origin		The origin to rotate around. This is optional, vec3(0, 0, 0) by default
		 * @return					The rotated vector coordinates
		*/
		static glm::vec3 rotate(glm::vec3 const& coordinate, float const& theta, glm::vec3 const& axis, glm::vec3 const& origin = glm::vec3(0));

		/* @brief Rotate some coordinate around a provided origin
		 * @param[in]	coordinate	The original input coordinate
		 * @param[in]	scale		The scale to scale the coordinates by
		 * @param[in]	origin		The origin to scale around. This is optional, vec3(0, 0, 0) by default
		 * @return					The rotated vector coordinates
		*/
		static glm::vec3 scale(glm::vec3 const& coordinate, glm::vec3 const& scale, glm::vec3 const& origin = glm::vec3(0));
	};

	/* @brief Camera object
	*/
	class Camera {
	private:
		// Positions
		glm::vec3 _pos;
		glm::vec3 _target;

		// Directions
		glm::vec3 _backward; // The tutorial says this is the backward direction
		glm::vec3 _right;

		// Camera stuff
		glm::mat4 _view;

		Camera& _updateRight();

	public:
		Camera(glm::vec3 pos = glm::vec3(0, 0, 0));

		// Positions
		glm::vec3 getPos();
		glm::vec3 getTarget();
		// Native directions
		glm::vec3 getBack();
		glm::vec3 getRight();
		glm::vec3 getUp();

		// Alternative for all the crap above?
		Camera& lookAt(glm::vec3 target);

		Camera& setPos(glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 3.0f));
		Camera& setTarget(glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f));
	};



	/* @brief Texture 
	*/
	class Texture {
	private:
		int width = 0;
		int height = 0;
		int channels = 0;
		
		unsigned int TID = 0;

	public:
		/* @brief Texture default constructor
		*/
		Texture();

		/* @brief Texture constructor. Load texture
		*/
		Texture(const char* path);

		/* @brief Texture destructor
		*/
		~Texture();

		/* @brief Load an image path into the texture
		*  @param[in]	path	The filepath to load
		*  @return				A reference to this texture object
		*/
		Texture& load(const char* path);

		/* @brief Destroy the image. Called on destructor. 
		*  @return A reference to this texture object
		*/
		Texture& destroy();

		/* @brief Get texture ID
		*  @return	The opengl texture ID of this texture object
		*/
		unsigned int getTexture();
	};



	/* @brief Shape object
	*/
	class Shape {
	protected:
		unsigned int vertCount = 0;
		std::vector<float> vertices;

		unsigned int indexCount = 0;
		std::vector<unsigned int> indices;
		
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;

		Texture* texture = nullptr;

	public:
		Shape();
		~Shape();

		/* @brief Push a single point to the shape.
		* @param[in]	vec		The vector of the point 
		* @param[in]	col		The color of the vertex
		* @param[in]	texPos	The texture position
		* @return 		A reference to this shape object
		*/
		Shape& pushPoint(glm::vec3 vec, glm::vec3 col, glm::vec2 texPos);

		/* @brief Push a triangle to the indicies list. A triangle is constructed of the following verticies which were defined with pushPoint
		 * @param[in] vertA	The A vertex index out of the point list, where the first point is 0
		 * @param[in] vertB	The B vertex index 
		 * @param[in] vertC	The C vertex index
		 * @return			A reference to this shape object
		*/
		Shape& pushTriangle(unsigned int vertA, unsigned int vertB, unsigned int vertC);

		/* @brief Set the texture 
		 * @param[in] newTexture	The texture object to set to
		 * @return					A reference to this shape object
		*/
		Shape& setTexture(Texture* newTexture);

		Shape& updateEBO();
		Shape& updateVAO();
		Shape& updateVBO();

		unsigned int getVAO();
		unsigned int getVBO();
		std::vector<float>& getVertices();

		virtual void draw();
	};




	/* @brief Rectangle object
	*/
	class Rectangle : public Shape {
	public:
		Rectangle();
	};

	/* @brief Rectangle object
	*/
	class Triangle : public Shape {
	public:
		Triangle();
	};





	enum ShaderType : uint8_t {
		FILE,
		RAW
	};

	/* @brief Shader object
	*/
	class Shader {
	private:
		unsigned int ID;
	public:
		Shader(const char* vertex, const char* fragment, ShaderType type);
		// use / activate the shader
		void use();
		// Utility uniform functions
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int vlaue) const;
		void setFloat(const std::string &name, float value) const;
		void setVec4(const std::string &name, std::vector<float> value) const;
		void setMat4(const std::string &name, glm::mat4* matrix) const;
	};




	/* @brief Window object
	 * @param HLGL_DRAW_WIREFRAMES 	Macro defined at compiler time to draw just wireframes. Not defined by default to draw normally
	*/
	class Window {
	private:
	    GLFWwindow* _window;

	    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	public:
	    // Default Constructor
	    Window();
	    ~Window();

	    // Open the window
	    Window& create();

	    // Close the window and clear memory
	    Window& destroy();

	    // Process user input
	    Window& processInput();

	    // Return if the window should close (true) or not (false)
	    bool shouldClose();

	    // Swap the front and back buffers
	    Window& bufferSwap();

	    // Poll GLFW events
	    Window& pollEvents();
	};
}

#endif // HL_OGLOPP_H
