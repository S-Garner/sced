#include "../external/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

#include "../external/glm/glm/glm.hpp"
#include "../external/glm/glm/gtc/matrix_transform.hpp"
#include "../external/glm/glm/gtc/type_ptr.hpp"
#include "core/Window.h"
#include "core/buffers/BufferData.h"
#include "input/Input.h"
#include "math/MathOps.h"
#include "objects/Mesh.h"
#include "objects/Vertex.h"

// Simple vertex structure: position + color
struct Vertz {
	float position[3];
	float color[3];
};

// Vertex Shader: No uniforms — uses attributes only
const char* vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aColor;

    out vec3 vColor;

    void main() {
        gl_Position = vec4(aPos, 1.0);
        vColor = aColor;
    }
)";

// Fragment Shader: No uniform — uses input color
const char* fragmentShaderSrc = R"(
    #version 330 core
    in vec3 vColor;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(vColor, 1.0);
    }
)";

// Compile & link shader program
GLuint createShaderProgram(const char* vtxSrc, const char* fragSrc) {
	auto compile = [](GLenum type, const char* src) {
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char log[512];
			glGetShaderInfoLog(shader, 512, nullptr, log);
			std::cerr << "Shader compile error:\n" << log << std::endl;
		}
		return shader;
	};

	GLuint vs = compile(GL_VERTEX_SHADER, vtxSrc);
	GLuint fs = compile(GL_FRAGMENT_SHADER, fragSrc);

	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetProgramInfoLog(program, 512, nullptr, log);
		std::cerr << "Program link error:\n" << log << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	Window window(800, 600, "SCEd base");
	if (!window.getWindow()) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window.getWindow());
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Input input;
	input.initialize(window.getWindow());

	Mesh mesh;
	Vertex v1, v2, v3;
	v1.set("position", {0.0f, 0.5f, 0.0f});
	v1.set("color", {1.0f, 0.0f, 0.0f});
	v2.set("position", {0.5f, -0.5f, 0.0f});
	v2.set("color", {0.0f, 1.0f, 0.0f});
	v3.set("position", {-0.5f, -0.5f, 0.0f});
	v3.set("color", {0.0f, 0.0f, 1.0f});
	Triangle tri(v1, v2, v3);
	mesh.addTriangle(tri);

	std::vector<float> verts = BufferData::getData(mesh);

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(),
				 GL_STATIC_DRAW);

	GLsizei stride = 6 * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
						  (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLuint shaderProgram =
		createShaderProgram(vertexShaderSrc, fragmentShaderSrc);

	while (!glfwWindowShouldClose(window.getWindow())) {
		glfwPollEvents();
		if (input.isKeyPressed(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window.getWindow(), true);
		}

		if (input.isKeyDown(GLFW_KEY_W)) {
			std::cout << "W key was just pressed!\n";
		}

		if (input.isKeyUp(GLFW_KEY_W)) {
			std::cout << "W key was just released!\n";
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window.getWindow());
		input.endFrame();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window.getWindow());
	glfwTerminate();

	return 0;
}
