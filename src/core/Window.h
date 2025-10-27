//
// Created by sethg on 10/4/2025.
//
#pragma once
#include <string>

struct GLFWwindow;

class Window
{
private:
    GLFWwindow* m_NativeWindow;

public:
    Window(int width, int height, std::string title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator = (const Window&) = delete;

    bool shouldClose() const;

    GLFWwindow* getNativeWindow() const { return m_NativeWindow; }
};