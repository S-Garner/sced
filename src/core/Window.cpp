//
// Created by sethg on 10/4/2025.
//

#include "Window.h"
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Window::Window(int width, int height, std::string title) : m_NativeWindow(nullptr)
{

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->m_NativeWindow = glfwCreateWindow(width,
        height,
        title.c_str(),
        NULL,
        NULL);

    if (!this->m_NativeWindow)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(this->m_NativeWindow);
    glfwSwapInterval(1); // For v-sync
}

Window::~Window()
{
    if (m_NativeWindow) {
        glfwDestroyWindow(m_NativeWindow);
    }
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(this->m_NativeWindow);
}

glm::vec2 Window::getDimensions() const {
    int width, height;
    glfwFramebufferSize(m_NativeWindow, &width, &height);
    return { static_cast<float>(width), static_cast<float>(height) };
}
