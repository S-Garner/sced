#include "Input.h"
#include <cstring>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Input::Input() : mouseX(0.0), mouseY(0.0) {
	memset(keys, false, sizeof(keys));
	memset(lastKeys, false, sizeof(lastKeys));
}

void Input::initialize(GLFWwindow* window) {
	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
}

void Input::endFrame() {
	memcpy(lastKeys, keys, sizeof(keys));
}

bool Input::isKeyPressed(int key) const {
	if (key >= 0 && key < MAX_KEYS) {
		return keys[key];
	}
	return false;
}

bool Input::isKeyDown(int key) const {
	if (key >= 0 && key < MAX_KEYS) {
		return keys[key] && !lastKeys[key];
	}
	return false;
}

bool Input::isKeyUp(int key) const {
	if (key >= 0 && key < MAX_KEYS) {
		return !keys[key] && lastKeys[key];
	}
	return false;
}

double Input::getMouseX() const {
	return mouseX;
}

double Input::getMouseY() const {
	return mouseY;
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Input* inputManager = static_cast<Input*>(glfwGetWindowUserPointer(window));
    if (inputManager && key >= 0 && key < MAX_KEYS) {
        if (action == GLFW_PRESS) {
            inputManager->keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            inputManager->keys[key] = false;
        }
    }
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	Input* inputManager = static_cast<Input*>(glfwGetWindowUserPointer(window));

	if (inputManager) {
		inputManager->mouseX = xpos;
		inputManager->mouseY = ypos;

		std::cout << "Mouse Position: (" << xpos << ", " << ypos << ")\n";
	}
}
