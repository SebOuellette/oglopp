#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

#endif