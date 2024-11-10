#include "../Headers/oglopp.h"

namespace oglopp {
	_HoneyLib_InitGL::_HoneyLib_InitGL() {
		// Instantiate the window
		//  Load up OpenGL version 3.3
		glfwInit();
		std::cout << "Init glfw" << std::endl;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		// This tells the compiler we want to use the core-profile; meaning a smaller subset of OpenGL features without backwards compatability features we don't need
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed on Mac OS
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}
	
	_HoneyLib_InitGL::~_HoneyLib_InitGL() {
		glfwTerminate();
		std::cout << "Terminate glfw" << std::endl;
	}

	_HoneyLib_InitGL _instance = _HoneyLib_InitGL();
}