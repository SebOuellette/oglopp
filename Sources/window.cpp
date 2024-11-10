#include "../Headers/oglopp.h"
#include <GLFW/glfw3.h>

namespace oglopp {
	// Callback function to automatically change viewport when window is resized
	void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	Window::Window() {
		this->_window = nullptr;

		
	}

	Window::~Window() {
	    this->destroy();
	}

	Window& Window::create() {
	    this->_window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
		if (_window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			this->destroy();
	        exit(1);
		}
		glfwMakeContextCurrent(this->_window);

		glfwShowWindow(this->_window);


		// Initialize GLAD before we call any OpenGL functions
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			this->destroy();
	        exit(1);
		}

		// Set the viewport
		//glViewport(0, 0, 800, 600);

#ifdef HLGL_DRAW_WIREFRAMES
		// Use wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

		// Callback function to automatically change viewport when window is resized
		glfwSetFramebufferSizeCallback(this->_window,this->framebuffer_size_callback);

		// Set the "background" colour of the window
		glClearColor(0.0, 0.0, 0.0, 0.0);

	    return *this;
	}

	Window& Window::destroy() {
		if (this->_window != nullptr) {
			std::cout << "Destroying window" << std::endl;

			glfwHideWindow(this->_window);

			glfwSetWindowShouldClose(this->_window, GLFW_TRUE);
			glfwDestroyWindow(this->_window);
			//this->_window = nullptr;
		}

	    return *this;
	}

	Window& Window::processInput() {
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(_window, GLFW_TRUE);
	    }

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
}
