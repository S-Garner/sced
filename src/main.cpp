#include "../external/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "../external/glm/glm/glm.hpp"
#include "../external/glm/glm/gtc/matrix_transform.hpp"
#include "../external/glm/glm/gtc/type_ptr.hpp"
#include <vector>
#include <cmath>
#include <thread>

#include "core/Window.h"
#include "core/buffers/BufferData.h"
#include "math/MathOps.h"
#include "objects/Mesh.h"
#include "objects/Vertex.h"
#include "input/Input.h"

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

    // Create an instance of a mesh
    Mesh mesh;

	//Keyboard detection
	Input input;

    Vertex v1, v2, v3;
    v1.set("position", {0.0f, 0.5f, 0.0f});
    v1.set("color", {1.0f, 0.0f, 0.0f});

    v2.set("position", {0.5f, -0.5f, 0.0f});
    v2.set("color", {1.0f, 0.0f, 0.0f});

    v3.set("position", {-0.5f, -0.5f, 0.0f});
    v3.set("color", {1.0f, 0.0f, 0.0f});

    Triangle tri(v1, v2, v3);
    mesh.addTriangle(tri);

    int counter = 0;
    for (auto* v : mesh.getAllVertices())
    {
        std::cout << "Point " + counter << std::endl;
        v->print();
        counter++;
    }
    std::cout << "\n" << std::endl;

    // Move entire mesh
    Math::translate(mesh, {0.3f, 0.3f});

    counter = 0;
    for (auto* v : mesh.getAllVertices())
    {
        std::cout << "Point " + counter << std::endl;
        v->print();
        counter++;
    }
    std::cout << "\n" << std::endl;

    // Rotate a specific vertices
    Math::rotate2D(mesh[0][1], 45.0f);

    counter = 0;
    for (auto* v : mesh.getAllVertices())
    {
        std::cout << "Point " + counter << std::endl;
        v->print();
        counter++;
    }
    std::cout << "\n" << std::endl;

    // Scale a specific triangle
    Math::scale(mesh[0], 1.5f);

    counter = 0;
    for (auto* v : mesh.getAllVertices())
    {
        std::cout << "Point " + counter << std::endl;
        v->print();
        counter++;
    }
    std::cout << "\n" << std::endl;

    for (auto* v : mesh.getAllVertices())
    {
        std::cout << "Point" << std::endl;
        v->print();
    }

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    Window window(800, 600, "SCEd base");
	
	thread inputThread(&Input::checkKeyboardInput, &input, window.getWindow());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return -1;
    }

    std::vector<float> verts = BufferData::getData(mesh);

    std::cout << "Vector elements" << std::endl;

    for (float value : verts)
    {
        std::cout << value << " ";
    }

    // Create and fill the buffer first
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    // Create and configure the VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Associate the pre-filled VBO with the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLsizei stride = 6 * sizeof(float); // 3 position + 3 color

    // Attribute 0: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);

    while (!glfwWindowShouldClose(window.getWindow())) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verts.size()));

        glfwSwapBuffers(window.getWindow());
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window.getWindow());
    glfwTerminate();
    return 0;
}
