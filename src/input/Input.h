#include <GLFW/glfw3.h>
#include <iostream>
#include "../core/Window.h"
using namespace std;

class Input {
public:
	void checkKeyboardInput(GLFWwindow* window);

	static void key_callback(GLFWwindow* window, int key, int scancode,
							 int action, int mods);
};

void Input::checkKeyboardInput(GLFWwindow* window) {
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
}

inline void Input::key_callback(GLFWwindow* window, int key, int scancode,
								int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	const char* keyName = glfwGetKeyName(key, scancode);

	if (action == GLFW_PRESS) {
		if (keyName != NULL) {
			cout << "Key pressed: " << keyName << endl;
		} else {
			// Fallback for special keys that don't have a name (like F1-F12)
			cout << "Special key pressed: " << key << endl;
		}
	} else if (action == GLFW_RELEASE) {
		if (keyName != NULL) {
			cout << "Key released: " << keyName << endl;
		} else {
			cout << "Special key released: " << key << endl;
		}
	} else if (action == GLFW_REPEAT) {	 // Handling the repeat action
		if (keyName != NULL) {
			cout << "Key repeated: " << keyName << endl;
		} else {
			cout << "Special key repeated: " << key << endl;
		}
	}
}
