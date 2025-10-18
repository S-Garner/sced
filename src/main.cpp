#include <cstdio>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Renderer/Shader/Shader.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/Transform.hpp"
#include "Renderer/Shapes.hpp"
#include "input/Input.h"

static void onGlfwError(int code, const char* desc) {
    std::fprintf(stderr, "[GLFW %d] %s\n", code, desc);
}
static void onResize(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

int main() {
    glfwSetErrorCallback(onGlfwError);
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(800, 600, "SCEd 2D Uniform Renderer", nullptr, nullptr);
    if (!win) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(win, onResize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(win);
        glfwTerminate();
        return 1;
    }

    Input input;
    input.initialize(win);
    Shader shader = Shader::fromFiles("Shader/config/flat.vert", "Shader/config/flat.frag");
    Renderer2D renderer;

    auto redRectVerts   = Shapes::makeRectangle({-0.5f,-0.5f}, {0.0f,0.0f}, {1,0,0});
    auto greenRectVerts = Shapes::makeRectangle({ 0.0f, 0.0f}, {0.5f,0.5f}, {0,1,0});

    ShapeHandle red   = renderer.addShape(redRectVerts.data(),   (int)redRectVerts.size(),   Transform::setIdentity());
    ShapeHandle green = renderer.addShape(greenRectVerts.data(), (int)greenRectVerts.size(),
                                          Transform::translate(Transform::setIdentity(), {0.25f,0.25f}));

    glm::mat4 redModel = Transform::setIdentity();
    glm::vec2 greenCenter = {0.25f, 0.25f};

    std::vector<ShapeHandle> onlyGreen = { green };

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();

        int w, h; glfwGetFramebufferSize(win, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float t = (float)glfwGetTime();

        glm::mat4 greenModel = Transform::setIdentity();
        greenModel = Transform::translate(greenModel, {0.25f,0.25f});
        greenModel = Transform::rotateZ_about(greenModel, 25.0, greenCenter);

        renderer.setModel(red, redModel);
        renderer.setModel(green, greenModel);

        glm::vec3 dynamic = { 0.5f + 0.5f*std::sin(t), 0.8f, 0.5f };
        renderer.setOverrideColor(green, dynamic);

        float aspect = (h==0) ? 1.0f : (float)w/(float)h;
        glm::mat4 vp = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

        renderer.drawShape(shader, vp, onlyGreen);

        renderer.drawAll(shader, vp);

        glfwSwapBuffers(win);

        if (input.isKeyPressed(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(win, true);
		}

		if (input.isKeyDown(GLFW_KEY_W)) {
			std::cout << "W key was just pressed!\n";
		}

		if (input.isKeyUp(GLFW_KEY_W)) {
			std::cout << "W key was just released!\n";
		}
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
