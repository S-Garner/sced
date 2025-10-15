#include <GLFW/glfw3.h>
#include "../core/Window.h"

#include <iostream>
using namespace std;

class Input {
public:
	void checkKeyboardInput(GLFWwindow*);
};

void Input::checkKeyboardInput(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cout << "The W Key was press";
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
