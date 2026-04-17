#ifndef OGLOPP_WINDOW_H
#define OGLOPP_WINDOW_H

#include "defines.h"
#include "camera.h"
#include "glad/gl.h"
//#include "oglopp/glad/gl.h"
#include <functional>


namespace oglopp {
	/*
	 * @brief The resize callback type. A function that takes the new width and height respectively as parameters
	 * @param [in] width	The new width
	 * @param [in] height	The new height
	 * @param [in] data		An optional pointer to settings.resizeCallbackPtr
	 */
	typedef std::function<void(int, int, void*)> ResizeCallback;

	/*
	 * @brief The resize callback type. A function that takes the new width and height respectively as parameters
	 * @param [in] key	The GLFW key pressed
	 * @param [in] scancode	A hardware/os code
	 * @param [in] action	Pressed or released
	 * @param [in] mods	Shift, ctrl?
	 * @param [in] data	An optional pointer to settings.keypressCallbackPtr
	 */
	typedef std::function<void(int, int, int, int, void*)> KeypressCallback;

	/**
	 * @brief The scrollwheel callback type. A function that takes the horizontal and vertical scroll magnitudes respectively as parameters
	 * @param[in] horiz	The amount scrolled horizontally
	 * @param[in] vert	The amount scrolled vertically
	 * @param[in] data	An optional pointer to settings.scrollCallbackPtr
	 */
	typedef std::function<void(double, double, void*)> ScrollCallback;


	/** @brief Window object
	 * @param HLGL_DRAW_WIREFRAMES 	Macro defined at compiler time to draw just wireframes. Not defined by default to draw normally
	*/
	class Window {
	public:
		enum DepthPass : uint16_t {
			ALWAYS		= GL_ALWAYS,
			NEVER		= GL_NEVER,
			LESS		= GL_LESS,
			EQUAL		= GL_EQUAL,
			LEQUAL		= GL_LEQUAL,
			GREATER		= GL_GREATER,
			NOTEQUAL	= GL_NOTEQUAL,
			GEQUAL		= GL_GEQUAL
		};

		enum StencilAction : uint16_t {
			KEEP		= GL_KEEP,	// The currently stored stencil value is kept
			ZERO		= GL_ZERO,	// The stencil value is set to 0
			REPLACE		= GL_REPLACE,	// The stencil value is replaced with the reference value set with Window::stencilFunc()
			INCR		= GL_INCR,	// The stencil value is increased by 1 if it is lower than the maximum value
			INCR_WRAP	= GL_INCR_WRAP,	// Same as INCR, but wraps it back to 0 as soon as the maximum value is exceeded
			DECR		= GL_DECR,	// The stencil value is decreased by 1 if it is higher than the minimum value
			DECR_WRAP	= GL_DECR_WRAP,	// Same as DECR, but wraps it to the maximum value if it ends up lower than 0
			INVERT		= GL_INVERT	// Bitwise inverts the current stencil buffer value
		};

		enum StencilFace : uint16_t {
			FRONT		= GL_FRONT,
			BACK		= GL_BACK,
			FRONT_AND_BACK	= GL_FRONT_AND_BACK
		};

		enum ClearMask : uint32_t {
			NONE		= 0,

			// The list of supported clear bits
			COLOR		= GL_COLOR_BUFFER_BIT,
			DEPTH		= GL_DEPTH_BUFFER_BIT,
			STENCIL		= GL_STENCIL_BUFFER_BIT,

			// bitwise OR of all clear bits
			ALL		= COLOR | DEPTH | STENCIL,
		};

		struct Settings {
			// Window options
			bool resizable = true;
			bool visible = true;
			GLFWmonitor* monitor = nullptr;
			GLFWwindow* share = nullptr;

			// A window can either render wireframes or polygons
			bool wireframes = false;
			
			// The window background color
			glm::vec4 clearColor = glm::vec4(0.0);

			// Depth buffer
			bool doStencilBuffer = false;
			bool doDepthBuffer = true;
			bool depthReadonly = false;
			DepthPass depthPass = DepthPass::LESS;

			// Face culling draws only the outer faces of shapes for optimization
			bool doFaceCulling = true;

			// Allow defining point size when drawing individual points rather than meshes
			bool modifyPointSize = false;

			// The resize callback to be run when the window is resized
			ResizeCallback resizeCallback = [](int, int, void*){};
			void* resizeCallbackPtr = nullptr; // Can be updated later on with setResizeCallbackDataPtr()

			KeypressCallback keypressCallback = [](int, int, int, int, void*){};
			void* keypressCallbackPtr = nullptr; // Can be updated later on with setKeypressCallbackDataPtr()
			
			ScrollCallback scrollCallback = [](double, double, void*){};
			void* scrollCallbackPtr = nullptr; // Can be updated later on with setScrollCallbackDataPtr()
		};


		// Default Constructor
		Window();
		~Window();

		/**
		 * @brief Create a window with some width and height
		 * @param[in]	width	The width (in pixels) of the window upon creation
		 * @param[in]	height	The height (in pixels) of the window upon creation
		 * @param[in]	title	The title of the window
		 * @param[in]	settings	A pointer to an optional list of settings for the window
		 * @return				A reference to this window object
		 */
		Window& create(unsigned int width, unsigned int height, const char* title, Settings const& settings );
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

		/** 
		 * @brief Get a reference to this object's camera
		 * @return A constant reference to this object's camera
		 */
		Camera& getCam();

		/** 
		 * @brief Get a pointer to the saved glfw window object
		 * @return	A pointer to the glfw window object
		 */
		GLFWwindow* getWindow();

		/** @brief Get the size of the window in pixels
		 * @param[out] width	The width of the window in pixels
		 * @param[out] height	The height of the window in pixels
		 * @return				A reference to this window object
		*/
		Window& getSize(int* width, int* height);


		/** @brief Lock and hide cursor in the window
		 * @return A reference to this window object
	 	*/
		Window& cursorCapture();

		/** @brief Release the cursor back to the user
		 * @return A reference to this window object
	 	*/
		Window& cursorRelease();

		/** @brief Check if the cursor is captured
		 * @return True if the cursor is captured by this window, false otherwise.
	 	*/
		bool isCursorCaptured();

		/** @brief Check if a glfw key is pressed down
		 * @param[in] key	The GLFW key code to check
		 * @return			True if the key is pressed, false otherwise
	 	*/
		bool keyPressed(uint16_t const& key);

		/** @brief Check if a glfw mouse button is pressed down
		 * @param[in] btn	The GLFW mouse button code to check. Starts with GLFW_MOUSE_
		 * @return			True if the button is pressed. False otherwise
		*/
		bool mousePressed(uint16_t const& key);

		/** @brief Get the cursor position
		 * @return The cursor X and Y position
	 	*/
		glm::dvec2 getCursorPos();

		/** @brief Set the cursor position
		 * @param[in] pos	The new position to set the cursor to
		 * @return 			A reference to this window object
	 	*/
		Window& setCursorPos(glm::dvec2 const& pos);

		/** @brief Clear the window
		 * @param[in] mask
		 * @return A reference to this window
	 	*/
		Window& clear(uint32_t maskXor = ClearMask::ALL);

		/** @brief Handle noclip movement
	 	*/
		Window& handleNoclip();

		/**
		 * @brief Disable the depth mask to pause writing to the depth buffer.
		 * @return A reference to this window
		 */
		Window& noDepth();
		
		/**
		 * @brief Enable the depth mask to continue writing to the depth buffer
		 * @return A reference to this window
		 */
		Window& useDepth();






		// All these stencil functions need to be moved into a Stencil class

		/**
		 * @brief Enable and disable stencil test on the fly
		 * @param[in] enabled	True to enable stencil test, false to disable
		 * @return		A reference to this window
		 */
		Window& enableStencil(bool enabled);

		/**
		 * @brief Disable the stencil mask to pause writing to the stencil buffer
		 * @return A reference to this window
		 */
		Window& readStencil();
		
		/**
		 * @brief Enable the stencil mask to continue writing to the stencil buffer
		 * @param[in] mask	An optional mask. Default value is 0xFF to use all bits. 
		 * @return A reference to this window
		 */
		Window& writeStencil(uint8_t mask = 0xFF);

		/**
		 * @brief Set the stencil condition for either the front, back or both sides
		 * @param[in] face	The face to update the stencil state for
		 * @param[in] condition	The test function to perform
		 * @param[in] reference	The reference value for the stencil test
		 * @param[in] mask	An optionalmask ANDed withboth reference and stored stencil
		 */
		Window& stencilFunc(StencilFace face, DepthPass condition, float reference, uint8_t mask = 0xFF);

		/**
		 * @brief Set the stencil condition and reference (and optional mask) for stencil tests
		 * @param[in] condition	The test function to perform
		 * @param[in] reference	The reference value for the stencil test
		 * @param[in] mask	An optionalmask ANDed withboth reference and stored stencil
		 */
		Window& stencilFunc(DepthPass condition, float reference, uint8_t mask = 0xFF);
		
		/**
		 * @brief Set the stencil operation to perform for either the front, back, or both sides
		 * @param[in] stFail	The action to perform upon failure of the stencil test. 
		 *			Was an object drawn to this fragment of the stencil test buffer?
		 * @param[in] dtFail	The action to perform upon failure of the depth test. 
		 *			Was an object drawn to this fragment of the depth test buffer?
		 * @param[in] pass	The action to perform upon success of the stencil function.
		 */
		Window& stencilOp(StencilFace face, StencilAction stFail, StencilAction dtFail, StencilAction pass);


		/**
		 * @brief Set the stencil operation to perform on reference or current value based on the func's pass or fail result
		 * @param[in] stFail	The action to perform upon failure of the stencil test. 
		 *			Was an object drawn to this fragment of the stencil test buffer?
		 * @param[in] dtFail	The action to perform upon failure of the depth test. 
		 *			Was an object drawn to this fragment of the depth test buffer?
		 * @param[in] pass	The action to perform upon success of the stencil function.
		 */
		Window& stencilOp(StencilAction stFail, StencilAction dtFail, StencilAction pass);


		//
		// Callbacks
		//

		/**
		 * @brief Resize the window
		 * @param[in] width	The width (in pixels) of the window
		 * @param[in] height	The height (in pixels) of the window
		 */
		Window& resize(int width, int height);
		
		/**
		 * @brief Set the resize callback data pointer
		 * @param[in] newPtr	The new pointer
		 */
		Window& setResizeCallbackDataPtr(void* newPtr);


		/**
		 * @brief Simulate a keypress event.
		 * @param[in] key	The GLFW key pressed
		 * @param[in] scancode	Key scancode
		 * @param[in] action	Key action (press, release)
		 * @param[in] mods	Key modifiers (ctrl, shift, alt)
		 */
		Window& keypress(int key, int scancode, int action, int mods);
	
		/**
		 * @brief Set the keypress callback data pointer
		 * @param[in] newPtr	The new pointer
		 */
		Window& setKeypressCallbackDataPtr(void* newPtr);

		/**
		 * @brief Simulate a scroll event. 
		 * @param[in] horiz	The horizontal scroll amount
		 * @param[in] vert	The vertical scroll amount
		 */
		Window& scroll(double horiz, double vert);

		/**
		 * @brief Set the scrollwheel callback data pointer
		 * @param[in] newPtr	The new pointer
		 */
		Window& setScrollCallbackDataPtr(void* newPtr);
	private:
		

		uint32_t clearMask;

		GLFWwindow* _window;

		Camera renderCamera;

		Window::Settings startSettings; // Copy of the settings used to start the window

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void keypress_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void scrollwheel_callback(GLFWwindow* window, double horiz, double vert);
	};
}

#endif
