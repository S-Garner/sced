//
// Created by sethg on 10/4/2025.
//

#include "Window.h"

#include <iostream>
#include <ostream>

Window::Window(int width, int height, std::string title)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(width,
        height,
        title.c_str(),
        NULL,
        NULL);

    if (!window)
    {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // For v-sync
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldCloseWindow()
{
    return glfwWindowShouldClose(this->window);
}

GLFWwindow* Window::getWindow()
{
    return this->window;
}

void Window::pollEvents()
{
    glfwPollEvents();
}