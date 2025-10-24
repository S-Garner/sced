#pragma once

struct GLFWwindow;

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
	    static constexpr int MAX_KEYS = 512;
		bool keys[MAX_KEYS];
		bool lastKeys[MAX_KEYS];
		double mouseX;
		double mouseY;

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};
