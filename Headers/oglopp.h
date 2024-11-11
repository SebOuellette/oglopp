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

//#define HLGL_VEC_INDEX			0
//#define HLGL_COL_INDEX			1
//#define HLGL_TEX_INDEX			2

#define HLGL_VEC_COMPONENTS 	3
#define HLGL_COL_COMPONENTS 	3
#define HLGL_TEX_COMPONENTS		2

#define HLGL_RENDER_FAR			(200.f)
#define HLGL_RENDER_NEAR		(0.01f)

//#define HLGL_STRIDE_ELEMENTS 	(HLGL_VEC_COMPONENTS + HLGL_COL_COMPONENTS + HLGL_TEX_COMPONENTS)
//#define HLGL_STRIDE_BYTES		(HLGL_STRIDE_ELEMENTS * sizeof(float))

#define HLGL_SHAPE_MAX_TEXTURES	(GL_MAX_TEXTURE_UNITS - GL_TEXTURE0)

#define HLGL_DEFAULT_FOV		65.f

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
		glm::vec3 _angle;

		// Directions
		glm::vec3 _backward; // The tutorial says this is the backward direction
		glm::vec3 _right;

		// Camera stuff
		glm::mat4 _view;

		float fov;

		Camera& _updateRight();

	public:
		Camera(glm::vec3 pos = glm::vec3(0, 0.0, 0.0));

		// Positions
		glm::vec3 getPos();
		glm::vec3 getTarget();
		// Native directions
		glm::vec3 getBack();
		glm::vec3 getRight();
		glm::vec3 getUp();
		float getFov();

		Camera& setFov(float);

		glm::vec3 const& getAngle();

		glm::mat4 const& getView();

		// Alternative for all the crap above?
		glm::mat4 const& lookAt(glm::vec3 target);

		Camera& setPos(glm::vec3 const& newPos = glm::vec3(0.0f, 0.0f, 3.0f));
		Camera& translate(glm::vec3 const& offset);
		Camera& setTarget(glm::vec3 const& newPos = glm::vec3(0.0f, 0.0f, 0.0f));
		Camera& setAngle(glm::vec3 const& newAngle = glm::vec3(0, 0, 0));
	};

	/* @brief Texture 
	*/
	class Texture {
	public:
		enum FileType {
			JPG,
			PNG
		};

	private:
		int width = -1;
		int height = -1;
		int channels = -1;
		
		unsigned int TID = 0;

		/* @brief Get color register that corresponds with some type. GL_RGB for jpg. GL_RGBA for png
		 * @return The register defining how to read the color
		*/
		uint16_t getTypeRegister(FileType type);

	public:

		/* @brief Texture default constructor
		*/
		Texture();

		/* @brief Texture constructor. Load texture
		*/
		Texture(const char* path, FileType type = FileType::JPG);

		/* @brief Texture destructor
		*/
		~Texture();

		/* @brief Load an image path into the texture
		*  @param[in]	path	The filepath to load
		*  @return				A reference to this texture object
		*/
		Texture& load(const char* path, FileType type = FileType::JPG);

		/* @brief Destroy the image. Called on destructor. 
		*  @return A reference to this texture object
		*/
		Texture& destroy();

		/* @brief Check if this texture object has loaded some image yet. Checks if the size is -1x-1x-1
		 * @return True if this object has loaded an image, false if this is empty
		*/
		bool isDefined();

		/* @brief Get the size of this texture. Will be -1x-1x-1 if this texture has not been defined
		 * @param[out] getWidth		A pointer to an integer where a width will be saved
		 * @param[out] getHeight	A pointer to an integer where the height will be saved
		 * @param[out] getChannels	Optionally get the channels. If nullptr, do not get the channels.
		*/
		Texture& getSize(int* getWidth, int* getHeight, int* getChannels = nullptr);

		/* @brief Get texture ID
		*  @return	The opengl texture ID of this texture object
		*/
		unsigned int getTexture();
	};


	enum ShaderType : uint8_t {
		FILE,	// const char* vertex and const char* fragment represent a path to the respective shaders. 
		RAW 	// const char* vertex and const char* fragment represent the fragment shader contents as a string. 
	};

	/* @brief Shader object
	*/
	class Shader {
	private:
		unsigned int ID;
	public:
		/* @brief Create a new shader
		*/
		Shader(const char* vertex, const char* fragment, ShaderType type);
		// use / activate the shader
		void use();
		// Utility uniform functions
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int vlaue) const;
		void setFloat(const std::string &name, float value) const;
		void setVec4(const std::string &name, glm::vec4 const& vector) const;
		void setMat4(const std::string &name, glm::mat4 const& matrix) const;
	};

	/* @brief Window object
	 * @param HLGL_DRAW_WIREFRAMES 	Macro defined at compiler time to draw just wireframes. Not defined by default to draw normally
	*/
	class Window {
	private:
	    GLFWwindow* _window;

		Camera renderCamera;

	    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	public:
	    // Default Constructor
	    Window();
	    ~Window();

	    /* @brief Create a window with some width and height
		 * @param[in]	width	The width (in pixels) of the window upon creation
		 * @param[in]	height	The height (in pixels) of the window upon creation
		 * @param[in]	title	The title of the window
		 * @return				A reference to this window object
		 */
		Window& create(unsigned int width, unsigned int height, const char* title);

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

		/* @brief Get a reference to this object's camera
		 * @return A constant reference to this object's camera
		*/
		Camera& getCam();

		/* @brief Get a pointer to the saved glfw window object
		* @return	A pointer to the glfw window object
		*/
		GLFWwindow* getWindow();

		/* @brief Get the size of the window in pixels
		 * @param[out] width	The width of the window in pixels
		 * @param[out] height	The height of the window in pixels
		 * @return				A reference to this window object
		*/
		Window& getSize(int* width, int* height);
	};




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

		Shape& updateEBO();
		Shape& updateVBO();

	public:
		Shape();
		~Shape();

#if 1

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
#endif

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
		 * @return	A reference to this shape object
		*/
		Shape& updateVAO(bool color = true, bool texture = true);

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

	/* @brief Rectangle object
	*/
	class Cube : public Shape {
	public:
		Cube();
	};
}

#endif // HL_OGLOPP_H
