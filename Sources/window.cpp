#include <iostream>

#include "oglopp/window.h"
#include "oglopp/init.h"

namespace oglopp {
	// Callback function to automatically change viewport when window is resized
	void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		pWindow->resize(width, height);
	}

	void Window::keypress_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		pWindow->keypress(key, scancode, action, mods);
	}

	void Window::scrollwheel_callback(GLFWwindow* window, double horiz, double vert) {
		Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		pWindow->scroll(horiz, vert);
	}




	Window::Window() {
		this->_window = nullptr;
		this->clearMask = GL_COLOR_BUFFER_BIT; // Initialize with the color buffer bit

	}

	Window::~Window() {
	    this->destroy();

		if (this->_window != nullptr) {
			glfwDestroyWindow(this->_window);
		}
	}

	/** @brief Create a window with some width and height
	 * @param[in]	width	The width (in pixels) of the window upon creation
	 * @param[in]	height	The height (in pixels) of the window upon creation
	 * @param[in]	title	The title of the window
	 * @param[in]	settings	A pointer to an optional list of settings for the window
	 * @return				A reference to this window object
	 */
	Window& Window::create(unsigned int width, unsigned int height, const char* title, Window::Settings const& settings) {
		// Copy the startup settings
		this->startSettings = settings;

		// FORCE the singleton to be initialized.
		// When linking liboglopp.a, the singleton often does not run, which means windows fail.
		// By just accessing a pointer to the singleton instance and storing it in a volatile pointer,
		// 	we can force the compiler to ensure the singleton instance exists before creating a window, without creating more initgl instances.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
		// This throws a compiler warning cause "inst" is 'unused'. Idk man what do you want me to do with it? It's being used as intended just here.
		// It's initialized in init.cpp:27, where it behaves as singleton, as intended. This warning can be ignored.
		volatile _HoneyLib_InitGL* inst = &_instance;
#pragma GCC diagnostic pop

		// Set the resizable bit based on the settings input
		glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, settings.visible ? GLFW_TRUE : GLFW_FALSE);

		// Create the window, pass monitor and share if provided
		this->_window = glfwCreateWindow(width, height, title, settings.monitor, settings.share);
		if (_window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			this->destroy();
	        	exit(1);
		}
		glfwMakeContextCurrent(this->_window);
		//glfwShowWindow(this->_window);


		// Initialize GLAD before we call any OpenGL functions
		if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			this->destroy();
	        exit(1);
		}

		// Wireframes mode
		if (settings.wireframes) {
			// Use wireframe mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			// Do not use wireframe mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Callback function to automatically change viewport when window is resized
		glfwSetWindowUserPointer(this->_window, this);
		glfwSetFramebufferSizeCallback(this->_window, this->framebuffer_size_callback);
		glfwSetKeyCallback(this->_window, this->keypress_callback);
		glfwSetScrollCallback(this->_window, this->scrollwheel_callback);

		// Set the "background" colour of the window
		glClearColor(settings.clearColor.r, settings.clearColor.g, settings.clearColor.b, settings.clearColor.a);

		// Check if the depth buffer should be enabled
		if (settings.doDepthBuffer) {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(settings.depthPass);

			// Apply the depth clear bit
			this->clearMask |= GL_DEPTH_BUFFER_BIT;
			settings.depthReadonly ? this->noDepth() : this->useDepth();
		} else {
			glDisable(GL_DEPTH_TEST);
		}

		// Stencil
		settings.doStencilBuffer ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);

		// Modifying point size
		if (settings.modifyPointSize) {
			glEnable(GL_PROGRAM_POINT_SIZE);
		} else {
			glDisable(GL_PROGRAM_POINT_SIZE);
		}

		// Face culling
		if (settings.doFaceCulling) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CW);
		} else {
			glDisable(GL_CULL_FACE);
		}

	    return *this;
	}

	Window& Window::create(unsigned int width, unsigned int height, const char* title) {
		// Create a default list of settings
		Window::Settings settings;

		return this->create(width, height, title, settings);
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

	/** @brief Get a reference to this object's camera
	* @return A constant reference to this object's camera
	*/
	Camera& Window::getCam() {
		return this->renderCamera;
	}

	/** @brief Get a pointer to the saved glfw window object
	 * @return	A pointer to the glfw window object
	*/
	GLFWwindow* Window::getWindow() {
		return this->_window;
	}

	/** @brief Get the size of the window in pixels
	* @param[out] width	The width of the window in pixels
	* @param[out] height	The height of the window in pixels
	* @return				A reference to this window object
	*/
	Window& Window::getSize(int* width, int* height) {
		glfwGetWindowSize(this->_window, width, height);
		return *this;
	}


	/** @brief Lock and hide cursor in the window
	 * @return A reference to this window object
 	*/
	Window& Window::cursorCapture() {
		glfwSetInputMode(this->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		return *this;
	}

	/** @brief Release the cursor back to the user
	 * @return A reference to this window object
 	*/
	Window& Window::cursorRelease() {
		glfwSetInputMode(this->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return *this;
	}

	/** @brief Check if the cursor is captured
	 * @return True if the cursor is captured by this window, false otherwise.
 	*/
	bool Window::isCursorCaptured() {
		return glfwGetInputMode(this->getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
	}


	/** @brief Check if a glfw key is pressed down
	 * @param[in] key	The GLFW key code to check. Starts with GLFW_KEY_
	 * @return			True if the key is pressed, false otherwise
 	*/
	bool Window::keyPressed(uint16_t const& key) {
		return glfwGetKey(this->getWindow(), key) == GLFW_PRESS;
	}

	/** @brief Check if a glfw mouse button is pressed down
	 * @param[in] btn	The GLFW mouse button code to check. Starts with GLFW_MOUSE_
	 * @return			True if the button is pressed. False otherwise
	*/
	bool Window::mousePressed(uint16_t const& key) {
		return glfwGetMouseButton(this->getWindow(), key) == GLFW_PRESS;
	}

	/** @brief Get the cursor position
	 * @return The cursor X and Y position
 	*/
	glm::dvec2 Window::getCursorPos() {
		glm::dvec2 ret;
		glfwGetCursorPos(this->getWindow(), &ret.x, &ret.y);

		return ret;
	}

	/** @brief Set the cursor position
	 * @param[in] pos	The new position to set the cursor to
	 * @return 			A reference to this window object
 	*/
	Window& Window::setCursorPos(glm::dvec2 const& pos) {
		glfwSetCursorPos(this->getWindow(), pos.x, pos.y);
		return *this;
	}

	/** @brief Clear the window
	 * @return A reference to this window
 	*/
	Window& Window::clear(uint32_t maskXor) {
		// Bitwise AND because by default maskXor will contain all clear bits.
		//  We only want to clear the bits supported in this window (as defined by the settings upon creation)
		// 
		// If we want to clear a specific buffer, we can define as such. But only the supported buffers can be cleared. 
		glClear(this->clearMask & maskXor);
		return *this;
	}

	/** @brief Handle noclip movement
 	*/
	Window& Window::handleNoclip() {
		// When escape is pressed...
		if (this->keyPressed(GLFW_KEY_ESCAPE)) {
			// Release the cursor
			this->cursorCapture();

			// If control is also pressed.. Destroy the window
			if (this->keyPressed(GLFW_KEY_LEFT_CONTROL)) {
				this->destroy();
			}
		}

		bool eventRecevied = false;
		{ // Keyboard
			double speed = 1.0;


			if (this->keyPressed(GLFW_KEY_LEFT_SHIFT)) {
				speed = 16.0;
				eventRecevied = true;
			} else {
				speed = 1.0;
			}

			if (this->keyPressed(GLFW_KEY_LEFT_CONTROL)) {
				this->getCam().translate(this->getCam().getUp() * -HLGL_NOCLIP_SPEED);
				eventRecevied = true;
			}

			if (this->keyPressed(GLFW_KEY_W)) {
				this->getCam().translate(this->getCam().getBack() * -HLGL_NOCLIP_SPEED * speed);
				eventRecevied = true;
			}
			if (this->keyPressed(GLFW_KEY_A)) {
				this->getCam().translate(this->getCam().getRight() * -HLGL_NOCLIP_SPEED * speed);
				eventRecevied = true;
			}
			if (this->keyPressed(GLFW_KEY_S)) {
				this->getCam().translate(this->getCam().getBack() * HLGL_NOCLIP_SPEED * speed);
				eventRecevied = true;
			}
			if (this->keyPressed(GLFW_KEY_D)) {
				this->getCam().translate(this->getCam().getRight() * HLGL_NOCLIP_SPEED * speed);
				eventRecevied = true;
			}
			if(this->keyPressed(GLFW_KEY_SPACE)) {
				this->getCam().translate(this->getCam().getUp() * HLGL_NOCLIP_SPEED);
				eventRecevied = true;
			}
		}

		// The cursor is trapped. So we can do window stuff now
		if (this->isCursorCaptured()) {
			// Get the cursor position then set it to the middle of the this->.. or bottom left idk but it works
			glm::dvec2 cursor = this->getCursorPos();
			this->setCursorPos({0.0, 0.0});

			this->getCam().aimBy(cursor.y, cursor.x);
		}

		if (eventRecevied) {
			this->cursorCapture();
			this->setCursorPos({0.0, 0.0});
		}

		if (this->keyPressed(GLFW_KEY_R)) {
			this->cursorRelease();
		}

		return *this;
	}


	/**
	 * @brief Disable the depth mask to pause writing to the depth buffer.
	 * @return A reference to this window
	 */
	Window& Window::noDepth() {
		glDepthMask(GL_FALSE);
		return *this;
	}
	
	/**
	 * @brief Enable the depth mask to continue writing to the depth buffer
	 * @return A reference to this window
	 */
	Window& Window::useDepth() {
		glDepthMask(GL_TRUE);
		return *this;
	}

	Window& Window::enableStencil(bool enabled) {
		enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
		return *this;
	}

	/**
	 * @brief Disable the stencil mask to pause writing to the stencil buffer
	 * @return A reference to this window
	 */
	Window& Window::readStencil() {
		glStencilMask(0x00); // Each bit ends up as 0 (disabling writes) - from opengl docs
		return *this;
	}
	
	/**
	 * @brief Enable the stencil mask to continue writing to the stencil buffer
	 * @param[in] mask	An optional mask. Default value is 0xFF to use all bits. 
	 * @return A reference to this window
	 */
	Window& Window::writeStencil(uint8_t mask) {
		glStencilMask(mask); 
		return *this;
	}
	
	/**
	 * @brief Set the stencil condition for either the front, back or both sides
	 * @param[in] face	The face to update the stencil state for
	 * @param[in] condition	The test function to perform
	 * @param[in] reference	The reference value for the stencil test
	 * @param[in] mask	An optionalmask ANDed withboth reference and stored stencil
	 */
	Window& Window::stencilFunc(StencilFace face, DepthPass condition, float reference, uint8_t mask) {
		glStencilFuncSeparate(face, condition, reference, mask);
		return *this;
	}


	/**
	 * @brief Set the stencil condition and reference (and optional mask) for stencil tests
	 * @param[in] condition	The depth pass
	 * @param[in] condition	The test function to perform
	 * @param[in] reference	The reference value for the stencil test
	 * @param[in] mask	An optionalmask ANDed withboth reference and stored stencil
	 */
	Window& Window::stencilFunc(DepthPass condition, float reference, uint8_t mask) {
		glStencilFunc(condition, reference, mask);
		return *this;
	}
	
	/**
	 * @brief Set the stencil operation to perform for either the front, back, or both sides
	 * @param[in] stFail	The action to perform upon failure of the stencil test. 
	 *			Was an object drawn to this fragment of the stencil test buffer?
	 * @param[in] dtFail	The action to perform upon failure of the depth test. 
	 *			Was an object drawn to this fragment of the depth test buffer?
	 * @param[in] pass	The action to perform upon success of the stencil function.
	 */
	Window& Window::stencilOp(StencilFace face, StencilAction stFail, StencilAction dtFail, StencilAction pass) {
		glStencilOpSeparate(face, stFail, dtFail, pass);	
		return *this;
	}

	/**
	 * @brief Set the stencil operation to perform on reference or current value based on the func's pass or fail result
	 * @param[in] stFail	The action to perform upon failure of the stencil test. Was an object drawn to this fragment of the stencil test buffer?
	 * @param[in] dtFail	The action to perform upon failure of the depth test. Was an object drawn to this fragment of the depth test buffer?
	 * @param[in] pass	The action to perform upon success of the stencil function.
	 */
	Window& Window::stencilOp(StencilAction stFail, StencilAction dtFail, StencilAction pass) {
		glStencilOp(stFail, dtFail, pass);
		return *this;
	}
	

	/**
	 * @brief Resize the window
	 * @param[in]	width	The width (in pixels) of the window
	 * @param[in]	height	The height (in pixels) of the window
	 */
	Window& Window::resize(int width, int height) {
		//std::cout << "Window resized [" << width << ", " << height << "]" << std::endl;
		glViewport(0, 0, width, height);

		// If the window is resized and moved around while the cursor is locked, free the cursor since it could be locked outside the window.
		glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// Call the user-defined resize callback to do some stuff
		this->startSettings.resizeCallback(width, height, this->startSettings.resizeCallbackPtr);

		return *this;
	}


	/**
	 * @brief Set the FBO resize callback data pointer
	 * @param[in] callback	The callback function
	 * @param[in] data	The new pointer
	 */
	Window& Window::setResizeCallback(ResizeCallback callback, void* data) {
		this->startSettings.resizeCallback = callback;
		this->startSettings.resizeCallbackPtr = data;

		return *this;
	}

	/**
	 * @brief Resize the window
	 * @param[in]	width	The width (in pixels) of the window
	 * @param[in]	height	The height (in pixels) of the window
	 */
	Window& Window::keypress(int key, int scancode, int action, int mods) {
		// Call the user-defined resize callback to do some stuff
		this->startSettings.keypressCallback(key, scancode, action, mods, this->startSettings.keypressCallbackPtr);

		return *this;
	}

	/**
	 * @brief Set the resize callback data pointer
	 * @param[in] callback	The callback function
	 * @param[in] data	The new pointer
	 */
	Window& Window::setKeypressCallback(KeypressCallback callback, void* data) {
		this->startSettings.keypressCallback = callback;
		this->startSettings.keypressCallbackPtr = data;

		return *this;
	}


	/**
	 * @brief Simulate a scroll event. 
	 * @param[in] horiz	The horizontal scroll amount
	 * @param[in] vert	The vertical scroll amount
	 */
	Window& Window::scroll(double horiz, double vert) {
		this->startSettings.scrollCallback(horiz, vert, this->startSettings.scrollCallbackPtr); 
		return *this;
	}

	/**
	 * @brief Set the scrollwheel callback data pointer
	 * @param[in] callback	The callback function
	 * @param[in] data	The new pointer
	 */
	Window& Window::setScrollCallback(ScrollCallback callback, void* data) {
		this->startSettings.scrollCallback = callback;
		this->startSettings.scrollCallbackPtr = data;
		return *this;
	}


}
