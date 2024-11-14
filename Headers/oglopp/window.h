#ifndef OGLOPP_WINDOW_H
#define OGLOPP_WINDOW_H

#include "defines.h"
#include "camera.h"

namespace oglopp {
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


		/* @brief Lock and hide cursor in the window
		 * @return A reference to this window object
	 	*/
		Window& cursorCapture();

		/* @brief Release the cursor back to the user
		 * @return A reference to this window object
	 	*/
		Window& cursorRelease();

		/* @brief Check if the cursor is captured
		 * @return True if the cursor is captured by this window, false otherwise.
	 	*/
		bool isCursorCaptured();



		/* @brief Check if a glfw key is pressed down
		 * @param[in] key	The GLFW key code to check
		 * @return			True if the key is pressed, false otherwise
	 	*/
		bool keyPressed(uint16_t const& key);

		/* @brief Get the cursor position
		 * @return The cursor X and Y position
	 	*/
		glm::dvec2 getCursorPos();

		/* @brief Set the cursor position
		 * @param[in] pos	The new position to set the cursor to
		 * @return 			A reference to this window object
	 	*/
		Window& setCursorPos(glm::dvec2 const& pos);
	};
}

#endif
