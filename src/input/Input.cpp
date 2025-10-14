#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

class Input {
public:
	int checkKeyboardInput();
};

void Input::checkKeyboardInput() {
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
