#include "../external/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "../external/glm/glm/glm.hpp"
#include "../external/glm/glm/gtc/matrix_transform.hpp"
#include "../external/glm/glm/gtc/type_ptr.hpp"
#include <vector>
#include <cmath>

#include "core/Window.h"
#include "Math/MathOps.h"
#include "objects/Mesh.h"
#include "objects/Vertex.h"

// Vertex + color struct (expandable later if needed)
struct Vertz {
    glm::vec3 position;
};

// Shaders
const char* vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    uniform mat4 uTransform;
    void main() {
        gl_Position = uTransform * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 uColor;
    void main() {
        FragColor = uColor;
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

    Mesh mesh;
    Vertex v1, v2, v3;
    v1.set("position", {0.0f, 0.5f});
    v2.set("position", {-0.5f, -0.5f});
    v3.set("position", {0.5f, -0.5f});

    Triangle tri(v1, v2, v3);

    mesh.addTriangle(tri);

    Math::translate(mesh, {0.3f, 0.3f});

    Math::rotate2D(mesh[0][1], 1.5f);

    Math::scale(mesh[0][1], 1.5f);

    for (auto* v : mesh.getAllVertices())
    {
        std::cout << "Point" << std::endl;
        v->print();
    }

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,
        600,
        "SCED Base",
        nullptr,
        nullptr);

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return -1;
    }

    // Triangle vertices
    std::vector<Vertz> vertices = {
        { glm::vec3( 0.0f,  0.5f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.0f) }
    };

    // Setup VAO/VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertz, position));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Shader program + uniforms
    GLuint shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    GLint transformLoc = glGetUniformLocation(shaderProgram, "uTransform");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");

    float time = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        time += 0.02f;
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(1.0f + 0.5f * std::sin(time), 1.0f, 1.0f));
        transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(std::sin(time) * 0.5f, std::sin(time) * 0.5f, 0));

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(colorLoc, 1.0f, 0.2f, 0.3f, 1.0f); // red-ish color

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
