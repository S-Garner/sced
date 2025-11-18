#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

#include "../Renderer/Shapes/RectangleShape.hpp"
#include "../Renderer/Shapes/CircleShape.hpp"
#include "../Renderer/Renderer2D.hpp"
#include "../Renderer/Shader/Shader.hpp"
#include "../Renderer/Transform.hpp"
#include "../Renderer/Vertex2D.hpp"

#include "../objects/SCObject.hpp"
#include "../core/Window.h"
#include "../input/Input.h"
#include "../color/SColor.hpp"


int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "SCED Pong Demo", nullptr, nullptr);
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    Renderer2D renderer;
    Shader shader = Shader::fromFiles("Shader/config/flat.vert", "Shader/config/flat.frag");

    Input input;
    input.initialize(window);

    int width = 0, height = 0;

    // ----------------------------------------
    // World coordinate bounds
    // ----------------------------------------
    float top    = 1.0f;
    float bottom = -1.0f;

    // ----------------------------------------
    // PADDLES
    // ----------------------------------------
    SCObject paddleLeft(&renderer);
    SCObject paddleRight(&renderer);

    glm::vec3 paddleColor = SColor::normalizeColor(230, 230, 230);

    RectangleShape paddleShape(
        { -0.05f, -0.2f },   // min
        {  0.05f,  0.2f },   // max
        paddleColor
    );

    paddleLeft.addShape(paddleShape);
    paddleRight.addShape(paddleShape);

    glm::vec2 paddleLeftPos  = { -1.3f, 0.0f };
    glm::vec2 paddleRightPos = {  1.3f, 0.0f };

    paddleLeft.setPosition(paddleLeftPos);
    paddleRight.setPosition(paddleRightPos);

    float paddleSpeed = 1.4f;

    // ----------------------------------------
    // BALL
    // ----------------------------------------
    SCObject ball(&renderer);

    CircleShape ballShape(
        { 0.0f, 0.0f },
        0.06f,
        48,
        SColor::normalizeColor(255, 255, 255)
    );

    ball.addShape(ballShape);

    glm::vec2 ballPos = { 0.0f, 0.0f };
    glm::vec2 ballVel = { 0.9f, 0.4f };

    // ----------------------------------------
    // TIME TRACKING
    // ----------------------------------------
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwGetFramebufferSize(window, &width, &height);
        if (height == 0) height = 1;
        float aspect = float(width) / float(height);

        glViewport(0, 0, width, height);
        glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 vp = glm::ortho(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);

        double now = glfwGetTime();
        float dt = float(now - lastTime);
        lastTime = now;

        // ----------------------------------------
        // PADDLE INPUT
        // ----------------------------------------
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            paddleLeftPos.y += paddleSpeed * dt;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            paddleLeftPos.y -= paddleSpeed * dt;

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            paddleRightPos.y += paddleSpeed * dt;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            paddleRightPos.y -= paddleSpeed * dt;

        // Clamp paddles
        if (paddleLeftPos.y > 0.8f) paddleLeftPos.y = 0.8f;
        if (paddleLeftPos.y < -0.8f) paddleLeftPos.y = -0.8f;

        if (paddleRightPos.y > 0.8f) paddleRightPos.y = 0.8f;
        if (paddleRightPos.y < -0.8f) paddleRightPos.y = -0.8f;

        paddleLeft.setPosition(paddleLeftPos);
        paddleRight.setPosition(paddleRightPos);

        // ----------------------------------------
        // BALL UPDATE
        // ----------------------------------------
        ballPos += ballVel * dt;
        ball.setPosition(ballPos);

        // Wall bounce
        if (ballPos.y > top) {
            ballPos.y = top;
            ballVel.y = -ballVel.y;
        }
        if (ballPos.y < bottom) {
            ballPos.y = bottom;
            ballVel.y = -ballVel.y;
        }

        // ----------------------------------------
        // PADDLE COLLISION
        // ----------------------------------------
        float paddleHalfHeight = 0.2f;
        float paddleHalfWidth  = 0.05f;
        float ballRadius       = 0.06f;

        // Left paddle collision
        if (ballPos.x - ballRadius < paddleLeftPos.x + paddleHalfWidth) {
            if (ballPos.y < paddleLeftPos.y + paddleHalfHeight &&
                ballPos.y > paddleLeftPos.y - paddleHalfHeight)
            {
                ballPos.x = paddleLeftPos.x + paddleHalfWidth + ballRadius;
                ballVel.x = -ballVel.x;
            }
        }

        // Right paddle collision
        if (ballPos.x + ballRadius > paddleRightPos.x - paddleHalfWidth) {
            if (ballPos.y < paddleRightPos.y + paddleHalfHeight &&
                ballPos.y > paddleRightPos.y - paddleHalfHeight)
            {
                ballPos.x = paddleRightPos.x - paddleHalfWidth - ballRadius;
                ballVel.x = -ballVel.x;
            }
        }

        // ----------------------------------------
        // SCORING (RESET)
        // ----------------------------------------
        if (ballPos.x > aspect + 0.5f || ballPos.x < -aspect - 0.5f) {
            ballPos = {0.0f, 0.0f};
            ballVel = {0.9f, 0.4f};
        }

        // ----------------------------------------
        // DRAW
        // ----------------------------------------
        paddleLeft.draw(shader, vp);
        paddleRight.draw(shader, vp);
        ball.draw(shader, vp);

        glfwSwapBuffers(window);
        input.endFrame();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
