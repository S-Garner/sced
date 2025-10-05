//
// Created by sethg on 10/4/2025.
//

#ifndef SCED_WINDOW_H
#define SCED_WINDOW_H
#include <string>

#include "GLFW/glfw3.h"


class Window
{
private:
    GLFWwindow* window;

public:
    Window(int width, int height, std::string title);
    ~Window();

    bool shouldCloseWindow();
    void pollEvents();
    void swapBuffers();

    GLFWwindow* getWindow();
};


#endif //SCED_WINDOW_H