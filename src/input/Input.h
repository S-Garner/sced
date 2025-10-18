#pragma once

#include <GLFW/glfw3.h>

class Input {
	public:
		Input();
		void initialize(GLFWwindow* window);

		void endFrame();
		bool isKeyPressed(int key) const;
		bool isKeyDown(int key) const;
		bool isKeyUp(int key) const;

		double getMouseX() const;
		double getMouseY() const;
	private:
		bool keys[GLFW_KEY_LAST];
		bool lastKeys[GLFW_KEY_LAST];
		double mouseX;
		double mouseY;

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};
