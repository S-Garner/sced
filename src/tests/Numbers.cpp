#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

#include "../Renderer/Shapes/IShape2D.hpp"
#include "../Renderer/Shapes/RectangleShape.hpp"
#include "../Renderer/Shapes/CircleShape.hpp"
#include "../Renderer/Shapes/EllipseShape.hpp"
#include "../Renderer/Shapes/RegularPolygonShape.hpp"

#include "../Renderer/Renderer2D.hpp"
#include "../Renderer/Shader/Shader.hpp"
#include "../Renderer/Transform.hpp"
#include "../Renderer/Vertex2D.hpp"

#include "../objects/SCObject.hpp"
#include "../core/Window.h"
#include "../input/Input.h"
#include "../color/SColor.hpp"
#include "../converter/SCArch.cpp"

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "SCObject Shape Test", nullptr, nullptr);
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    Renderer2D renderer;
    Shader shader = Shader::fromFiles("Shader/config/flat.vert", "Shader/config/flat.frag");

    SCObject one(&renderer);

    RectangleShape upperRect(
        {-0.2f, -0.25},
        {0.1f, -0.1f},
        SColor::normalizeColor(255, 255, 255)
    );



    one.addShape(upperRect);

    int width, height;

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (float)width / (float)height;

        glm::mat4 vp = glm::ortho(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);

        one.draw(shader, vp);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}