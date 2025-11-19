#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "../input/Input.h"
#include "../input/ScreenToWorld.hpp"

struct FrameState {
    glm::vec2 worldPos;
    bool mouseDown;
    bool mouseJustPressed;
    bool overButton;
};

inline FrameState updateFrameInput(class Input& input, GLFWwindow* window,
                                   int width, int height, float aspect, bool mouseWasDown) {
    FrameState fi{};
    fi.worldPos = screenToWorld(input.getMouseX(), input.getMouseY(), width, height, aspect);
    fi.mouseDown = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    fi.mouseJustPressed = (fi.mouseDown && !mouseWasDown);
    return fi;
}
