#include "../Headers/window.hpp"

// Callback function to automatically change viewport when window is resized
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

Window::Window() {
    // Instantiate the window
	//  Load up OpenGL version 3.3
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// This tells the compiler we want to use the core-profile; meaning a smaller subset of OpenGL features without backwards compatability features we don't need
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAR, GL_TRUE); // Needed on Mac OS
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
}

Window::~Window() {
    this->destroy();
}

Window& Window::create() {
    _window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
	if (_window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		this->destroy();
        exit(1);
	}
	glfwMakeContextCurrent(_window);

	// Initialize GLAD before we call any OpenGL functions
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		this->destroy();
        exit(1);
	}

	// Set the viewport
	glViewport(0, 0, 800, 600);

	// Use wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Callback function to automatically change viewport when window is resized
	glfwSetFramebufferSizeCallback(_window,this->framebuffer_size_callback);

	// Set the "background" colour of the window
	glClearColor(0.0, 0.0, 0.0, 0.0);

    return *this;
}

Window& Window::destroy() {
    glfwTerminate();

    return *this;
}

Window& Window::processInput() {
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(_window, true);
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