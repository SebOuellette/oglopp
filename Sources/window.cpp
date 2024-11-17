#include <iostream>

#include "oglopp/window.h"
#include "oglopp/init.h"

namespace oglopp {
	// Callback function to automatically change viewport when window is resized
	void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		//std::cout << "Window resized [" << width << ", " << height << "]" << std::endl;
		glViewport(0, 0, width, height);

		// If the window is resized and moved around while the cursor is locked, free the cursor since it could be locked outside the window.
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	Window::Window() {
		this->_window = nullptr;


	}

	Window::~Window() {
	    this->destroy();

		if (this->_window != nullptr) {
			glfwDestroyWindow(this->_window);
		}
	}

	/* @brief Create a window with some width and height
	 * @param[in]	width	The width (in pixels) of the window upon creation
	 * @param[in]	height	The height (in pixels) of the window upon creation
	 * @param[in]	title	The title of the window
	 * @return				A reference to this window object
	 */
	Window& Window::create(unsigned int width, unsigned int height, const char* title) {
		// FORCE the singleton to be initialized.
		// When linking liboglopp.a, the singleton often does not run, which means windows fail.
		// By just accessing a pointer to the singleton instance and storing it in a volatile pointer,
		// 	we can force the compiler to ensure the singleton instance exists before creating a window, without creating more initgl instances.
		volatile _HoneyLib_InitGL* inst = &_instance;
		// This throws a compiler warning cause "inst" is unused. Idk man what do you want me to do with it? It's being used as intended just here.

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	    this->_window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (_window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			this->destroy();
	        exit(1);
		}
		glfwMakeContextCurrent(this->_window);
		glfwShowWindow(this->_window);


		// Initialize GLAD before we call any OpenGL functions
		if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			this->destroy();
	        exit(1);
		}

#ifdef HLGL_DRAW_WIREFRAMES
		// Use wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

		// Callback function to automatically change viewport when window is resized
		glfwSetFramebufferSizeCallback(this->_window, this->framebuffer_size_callback);

		// Set the "background" colour of the window
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PROGRAM_POINT_SIZE);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);

	    return *this;
	}

	Window& Window::destroy() {
		if (this->_window != nullptr) {
			std::cout << "Destroying window" << std::endl;

			glfwHideWindow(this->_window);

			glfwSetWindowShouldClose(this->_window, GLFW_TRUE);
			//this->_window = nullptr;
		}

	    return *this;
	}

	Window& Window::processInput() {
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(_window, GLFW_TRUE);
	    }

		// Additional inputs are processed per implementation

	    return *this;
	}

	bool Window::shouldClose() {
	    return glfwWindowShouldClose(_window);
	}

	Window& Window::bufferSwap() {
	    glfwSwapBuffers(_window);

	    return *this;
	}

	Window& Window::pollEvents() {
	    glfwPollEvents();

	    return *this;
	}

	/* @brief Get a reference to this object's camera
	* @return A constant reference to this object's camera
	*/
	Camera& Window::getCam() {
		return this->renderCamera;
	}

	/* @brief Get a pointer to the saved glfw window object
	 * @return	A pointer to the glfw window object
	*/
	GLFWwindow* Window::getWindow() {
		return this->_window;
	}

	/* @brief Get the size of the window in pixels
	* @param[out] width	The width of the window in pixels
	* @param[out] height	The height of the window in pixels
	* @return				A reference to this window object
	*/
	Window& Window::getSize(int* width, int* height) {
		glfwGetWindowSize(this->_window, width, height);
		return *this;
	}


	/* @brief Lock and hide cursor in the window
	 * @return A reference to this window object
 	*/
	Window& Window::cursorCapture() {
		glfwSetInputMode(this->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		return *this;
	}

	/* @brief Release the cursor back to the user
	 * @return A reference to this window object
 	*/
	Window& Window::cursorRelease() {
		glfwSetInputMode(this->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return *this;
	}

	/* @brief Check if the cursor is captured
	 * @return True if the cursor is captured by this window, false otherwise.
 	*/
	bool Window::isCursorCaptured() {
		return glfwGetInputMode(this->getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
	}


	/* @brief Check if a glfw key is pressed down
	 * @param[in] key	The GLFW key code to check
	 * @return			True if the key is pressed, false otherwise
 	*/
	bool Window::keyPressed(uint16_t const& key) {
		return glfwGetKey(this->getWindow(), key) == GLFW_PRESS;
	}

	/* @brief Get the cursor position
	 * @return The cursor X and Y position
 	*/
	glm::dvec2 Window::getCursorPos() {
		glm::dvec2 ret;
		glfwGetCursorPos(this->getWindow(), &ret.x, &ret.y);

		return ret;
	}

	/* @brief Set the cursor position
	 * @param[in] pos	The new position to set the cursor to
	 * @return 			A reference to this window object
 	*/
	Window& Window::setCursorPos(glm::dvec2 const& pos) {
		glfwSetCursorPos(this->getWindow(), pos.x, pos.y);
		return *this;
	}
}
