#include "Application.h"
#include "../core/Window.h"
#include "../input/Input.h"
#include "../Renderer/Renderer2D.hpp"
#include "../Renderer/Shader/Shader.hpp"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Renderer/Transform.hpp"
#include "../Renderer/Shapes.hpp"

// Callbacks
static void onGlfwError(int error, const char* desc) { std::fprintf(stderr, "[GLFW %d] %s\n", error, desc); }
static void onResize(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

Application::Application(int width, int height, const char* title)
    : m_Width(width), m_Height(height), m_Title(title) {}

Application::~Application() {}

void Application::run() {
    try {
        initialize();
        mainLoop();
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    cleanup();
}

void Application::initialize() {
    std::cout << "Initializing application..." << std::endl;
    glfwSetErrorCallback(onGlfwError);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    m_Window = std::make_unique<Window>(m_Width, m_Height, m_Title);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    m_Input = std::make_unique<Input>();
    m_Renderer = std::make_unique<Renderer2D>();

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetFramebufferSizeCallback(m_Window->getNativeWindow(), onResize);
    m_Input->initialize(m_Window->getNativeWindow());

    m_Shader = std::make_unique<Shader>(Shader::fromFiles("Shader/config/flat.vert", "Shader/config/flat.frag"));
    std::cout << "Initialization successful." << std::endl;
}

void Application::cleanup() {
    std::cout << "Cleaning up..." << std::endl;
    m_Shader.reset();
    m_Renderer.reset();
    m_Input.reset();
    m_Window.reset();
    glfwTerminate();
}

void Application::mainLoop() {

    std::vector<Vertex2D> baseVerts = Shapes::makeRectangle({-0.5f,-0.5f}, {0.0f,0.0f}, {1,0,0});
    auto redRectVerts   = Shapes::makeRectangle({-0.5f,-0.5f}, {0.0f,0.0f}, {1,0,0});
    auto greenRectVerts = Shapes::makeRectangle({ 0.0f, 0.0f}, {0.5f,0.5f}, {0,1,0});

    ShapeHandle red   = m_Renderer->addShape(redRectVerts.data(),   (int)redRectVerts.size(),   Transform::setIdentity());
    ShapeHandle green = m_Renderer->addShape(greenRectVerts.data(), (int)greenRectVerts.size(),
        Transform::translate(Transform::setIdentity(), {0.25f,0.25f}));

    glm::vec2 greenCenter = {0.25f, 0.25f};

    while (!m_Window->shouldClose()) {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float t = (float)glfwGetTime();
        int w, h;
        glfwGetFramebufferSize(m_Window->getNativeWindow(), &w, &h);

        glm::mat4 greenModel = Transform::setIdentity();
        greenModel = Transform::translate(greenModel, greenCenter);
        greenModel = Transform::rotateZ_about(greenModel, 25.0f, greenCenter);
        greenModel = Transform::translate(greenModel, -greenCenter);
        greenModel = Transform::translate(greenModel, greenCenter);

        glm::vec3 dynamicColor = { 0.5f + 0.5f*std::sin(t), 0.5f + 0.5f*std::cos(t), 0.7f };

        m_Renderer->setModel(green, greenModel);
        m_Renderer->setOverrideColor(green, dynamicColor);

        float aspect = (h == 0) ? 1.0f : (float)w / (float)h;
        glm::mat4 vp = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

        m_Renderer->drawAll(*m_Shader, vp);

        glfwSwapBuffers(m_Window->getNativeWindow());
        if (m_Input->isKeyPressed(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(m_Window->getNativeWindow(), true);
        }

        if (m_Input->isKeyDown(GLFW_KEY_W)) {
			std::cout << "W key was just pressed!\n";
		}

		if (m_Input->isKeyUp(GLFW_KEY_W)) {
			std::cout << "W key was just released!\n";
		}

        m_Input->endFrame();
    }
}