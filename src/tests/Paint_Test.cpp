#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

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

#include "../converter/SCArch.cpp"
#include "../color/SColor.hpp"


#include "../ui/elements/SCButton.hpp"

struct Brush {
    float radius;
    float spacing;
    glm::vec3 color;
    int segments;

    std::vector<Vertex2D> generateVertices(const glm::vec2& center) const {
        CircleShape circle(center, radius, segments, color);
        return circle.generateVertices();
    }
};

glm::vec2 screenToWorld(double mx, double my, int width, int height) {
    float x = float(mx) / float(width);
    float y = float(my) / float(height);

    x = x * 2.0f - 1.0f;
    y = -y * 2.0f + 1.0f;

    return { x, y };
}

FrameState updateMouseWorld(Input& input, GLFWwindow* window,
                            int width, int height, float aspect)
{
    FrameState fs{};

    double mx = input.getMouseX();
    double my = input.getMouseY();

    glm::vec2 world = screenToWorld(mx, my, width, height);
    world.x *= aspect;

    fs.worldPos   = world;
    fs.mouseDown  = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

    return fs;
}

void handlePainting(FrameState& fs,
                    Brush& brush,
                    SCObject& stroke,
                    glm::vec2& lastPlacedPos,
                    bool& lastPosValid,
                    bool mouseWasDown)
{
    if (fs.mouseDown && !mouseWasDown) {
        auto verts = brush.generateVertices(fs.worldPos);
        stroke.addShape(verts);
        lastPlacedPos = fs.worldPos;
        lastPosValid = true;
    }
    else if (fs.mouseDown && mouseWasDown) {
        if (lastPosValid) {
            float dist = glm::distance(fs.worldPos, lastPlacedPos);
            if (dist >= brush.spacing) {
                auto verts = brush.generateVertices(fs.worldPos);
                stroke.addShape(verts);
                lastPlacedPos = fs.worldPos;
            }
        } else {
            auto verts = brush.generateVertices(fs.worldPos);
            stroke.addShape(verts);
            lastPlacedPos = fs.worldPos;
            lastPosValid = true;
        }
    }
    else if (!fs.mouseDown && mouseWasDown) {
        lastPosValid = false;
    }
}


int main() {
    // --- init stuff unchanged ---
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //GLFWwindow* window = glfwCreateWindow(800, 600, "SCED Paint Test", nullptr, nullptr);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "SCED Paint Test", nullptr, nullptr);
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    Renderer2D renderer;
    Shader shader = Shader::fromFiles("Shader/config/flat.vert", "Shader/config/flat.frag");

    Input input;
    input.initialize(window);

    // Background
    SCObject background(&renderer);
    {
        RectangleShape fullScreenRect(
            { -1.333f, -1.0f },
            {  1.333f,  1.0f },
            { 0.05f, 0.05f, 0.08f }
        );
        //background.addShape(fullScreenRect);
    }

    SCObject newBackground(&renderer);
    RectangleShape newFullScreen(
        SCArch::Rect(10.0, 10.0, {-2.0f, 2.0f}, SColor::normalizeColor(245, 245, 245))
    );
    background.addShape(newFullScreen);

    // Stroke frames
    std::vector<SCObject> strokes;
    int currentFrame = 0;

    // create the first empty frame
    strokes.emplace_back(&renderer);
    SCObject* active = &strokes[currentFrame];

    // Brush
    Brush brush;
    brush.radius   = 0.01f;
    brush.spacing  = 0.00001f;
    //brush.color    = glm::vec3(240/255.f, 240/255.f, 255/255.f);
    brush.color = SColor::normalizeColor(0, 0, 0);
    brush.segments = 36;

    bool      mouseWasDown  = false;
    glm::vec2 lastPlacedPos = glm::vec2(0.0f, 0.0f);
    bool      lastPosValid  = false;

    // --- red button setup unchanged ---
    SCObject redButton(&renderer);
    auto rectShape = CircleShape({-1.7f, .9f}, .05f, 64, SColor::normalizeColor(255, 163, 163));
    auto redBtnHandle = redButton.addShape(rectShape);
    SCButton redBtn(&redButton, redBtnHandle, &renderer);
    bool toggled = false;
    redBtn.setCallback([&](){
        toggled = !toggled;
        if (toggled) {
            brush.color = glm::vec3(1.0f, 0.2f, 0.2f);
            redButton.setShapeColor(redBtnHandle, glm::vec3(1.0f, 0.2f, 0.2f));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            redButton.setShapeColor(redBtnHandle, SColor::normalizeColor(255, 163, 163));
        }
        std::cout << "red button clicked\n";
    });


    SCObject blueButton(&renderer);
    auto blueCirc = CircleShape({-1.56f, .9f}, .05f, 64, SColor::normalizeColor(163, 163, 255));
    auto blueBtnHandle = blueButton.addShape(blueCirc);
    SCButton blueBtn(&blueButton, blueBtnHandle, &renderer);
    bool toggledBlue = false;
    blueBtn.setCallback([&](){
        toggledBlue = !toggledBlue;
        if (toggledBlue) {
            brush.color = SColor::normalizeColor(0, 0, 255);
            blueButton.setShapeColor(blueBtnHandle, SColor::normalizeColor(0, 0, 255));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            blueButton.setShapeColor(blueBtnHandle, SColor::normalizeColor(163, 163, 255));
        }
        std::cout << "blue button clicked\n";
    });

    SCObject yellowButton(&renderer);

    auto yellowCirc = CircleShape({-1.7f, 0.74f}, .05f, 64, SColor::normalizeColor(220, 220, 163));
    auto yellowBtnHandle = yellowButton.addShape(yellowCirc);

    SCButton yellowBtn(&yellowButton, yellowBtnHandle, &renderer);
    bool toggledYellow = false;
    yellowBtn.setCallback([&](){
        toggledYellow = !toggledYellow;
        if (toggledYellow) {
            brush.color = SColor::normalizeColor(255, 255, 0);
            yellowButton.setShapeColor(yellowBtnHandle, SColor::normalizeColor(255, 255, 0));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            yellowButton.setShapeColor(yellowBtnHandle, SColor::normalizeColor(200, 200, 163));
        }
    });

    SCObject greenButton(&renderer);

    auto greenCirc = CircleShape({-1.56f, 0.74}, .05f, 64, SColor::normalizeColor(163, 255, 163));
    auto greenBtnHandle = greenButton.addShape(greenCirc);

    SCButton greenBtn(&greenButton, greenBtnHandle, &renderer);
    bool toggledGreen = false;
    greenBtn.setCallback([&](){
        toggledGreen = !toggledGreen;
        if (toggledGreen) {
            brush.color = SColor::normalizeColor(0, 255, 0);
            greenButton.setShapeColor(greenBtnHandle, SColor::normalizeColor(0, 255, 0));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            greenButton.setShapeColor(greenBtnHandle, SColor::normalizeColor(163, 255, 163));
        }
    });

    SCObject upArrowButton(&renderer);
    glm::vec2 upCenter = {-1.4f, 0.9f};

    CircleShape upBgCircleLocal(
        {0.0f, 0.0f},
        0.08f,
        64,
        SColor::normalizeColor(255,255,255)
    );

    auto upBgVerts = upBgCircleLocal.generateVertices();
    for (auto& v : upBgVerts) {
        v.pos += upCenter;
    }

    std::vector<Vertex2D> upArrow = {
        { glm::vec2( 0.00f,  0.04f), glm::vec3(0,0,0) },
        { glm::vec2( 0.03f, -0.02f), glm::vec3(0,0,0) },
        { glm::vec2(-0.03f, -0.02f), glm::vec3(0,0,0) },
    };

    for (auto& v : upArrow) {
        v.pos += upCenter;
    }

    ShapeHandle arrowU = upArrowButton.addShape(upArrow);
    ShapeHandle bgU = upArrowButton.addShape(upBgVerts);
    SCButton arrowUpBtn(&upArrowButton, bgU, &renderer);

    arrowUpBtn.setCallback([&]() {
        int percentage_to_subtract = 10;

        double ten_percent = (percentage_to_subtract / 100.0) * brush.radius;
        brush.radius += ten_percent;

        upArrowButton.setShapeColor(bgU, SColor::normalizeColor(255,255,0));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        upArrowButton.setShapeColor(bgU, SColor::normalizeColor(255,255,255));
        
        std::cout << "Brush radius: " << brush.radius << std::endl;
    });

    SCObject dwnArrowButton(&renderer);
    glm::vec2 dwnCenter = {-1.4f, 0.7f };

    CircleShape bgCircleLocal(
        {0.0f, 0.0f},
        0.08f,
        64,
        SColor::normalizeColor(255,255,255)
    );

    auto bgVerts = bgCircleLocal.generateVertices();
    for (auto& v : bgVerts) {
        v.pos += dwnCenter;
    }

    std::vector<Vertex2D> downArrow = {
        { glm::vec2(-0.03f, 0.02f), glm::vec3(0,0,0) },
        { glm::vec2(0.03f, 0.02f), glm::vec3(0,0,0) },
        { glm::vec2(0.0f, -0.04f), glm::vec3(0,0,0) }
    };

    for (auto& v : downArrow) {
        v.pos += dwnCenter;
    }

    ShapeHandle arrowH = dwnArrowButton.addShape(downArrow);
    ShapeHandle bgH    = dwnArrowButton.addShape(bgVerts);
    SCButton arrowBtn(&dwnArrowButton, bgH, &renderer);

    arrowBtn.setCallback([&](){
        int percentage_to_subtract = 10;

        double ten_percent = (percentage_to_subtract / 100.0) * brush.radius;
        brush.radius -= ten_percent;

        dwnArrowButton.setShapeColor(bgH, SColor::normalizeColor(255,255,0));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        dwnArrowButton.setShapeColor(bgH, SColor::normalizeColor(255,255,255));

        std::cout << "Brush radius: " << brush.radius << std::endl;
    });

    int width = 0, height = 0;

    // edge state for frames
    bool prevRight = false;
    bool prevLeft  = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwGetFramebufferSize(window, &width, &height);
        if (height == 0) height = 1;
        float aspect = float(width) / float(height);

        glViewport(0, 0, width, height);
        glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        FrameState fs = updateMouseWorld(input, window, width, height, aspect);
        fs.mouseJustPressed = (fs.mouseDown && !mouseWasDown);

        // FRAME INDEX LOGIC
        int rightState = glfwGetKey(window, GLFW_KEY_RIGHT);
        int leftState  = glfwGetKey(window, GLFW_KEY_LEFT);

        bool justRight = (rightState == GLFW_PRESS && !prevRight);
        bool justLeft  = (leftState  == GLFW_PRESS && !prevLeft);

        if (justRight) {
            // if we're on the last frame, create a new one
            if (currentFrame == (int)strokes.size() - 1) {
                strokes.emplace_back(&renderer);
            }
            currentFrame++;
        }

        if (justLeft) {
            if (currentFrame > 0) {
                currentFrame--;
            }
        }

        prevRight = (rightState == GLFW_PRESS);
        prevLeft  = (leftState  == GLFW_PRESS);

        active = &strokes[currentFrame];

        // old mouse state
        redBtn.update(fs, mouseWasDown);
        blueBtn.update(fs, mouseWasDown);
        yellowBtn.update(fs, mouseWasDown);
        greenBtn.update(fs, mouseWasDown);
        arrowBtn.update(fs, mouseWasDown);
        arrowUpBtn.update(fs, mouseWasDown);

        // Ignore painting when mouse is over the buttons
        if (!redBtn.contains(fs.worldPos) &&
            !blueBtn.contains(fs.worldPos) &&
            !arrowBtn.contains(fs.worldPos) &&
            !greenBtn.contains(fs.worldPos) &&
            !yellowBtn.contains(fs.worldPos) &&
            !arrowUpBtn.contains(fs.worldPos))
        {
            handlePainting(fs, brush, *active, lastPlacedPos, lastPosValid, mouseWasDown);
        }

        mouseWasDown = fs.mouseDown;

        glm::mat4 vp = glm::ortho(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);

        background.draw(shader, vp);
        active->draw(shader, vp);

        redBtn.draw(shader, vp);
        blueBtn.draw(shader, vp);
        yellowBtn.draw(shader, vp);
        greenBtn.draw(shader, vp);
        arrowBtn.draw(shader, vp);
        arrowUpBtn.draw(shader, vp);

        glfwSwapBuffers(window);
        input.endFrame();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
