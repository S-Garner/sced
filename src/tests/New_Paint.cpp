// New_Paint.cpp
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

#include "../ui/FrameState.hpp"
#include "../input/ScreenToWorld.hpp"
#include "../ui/UIManager.hpp"
#include "../scene/LayerStack.hpp"

// -------------------------------------------------------------
// Brush definition
// -------------------------------------------------------------
struct Brush {
    float radius;
    float spacing;
    glm::vec3 color;
    int   segments;

    std::vector<Vertex2D> generateVertices(const glm::vec2& center) const {
        CircleShape circle(center, radius, segments, color);
        return circle.generateVertices();
    }
};

// -------------------------------------------------------------
// Painting logic (unchanged semantics, now using LayerStack)
// -------------------------------------------------------------
void handlePainting(const FrameState& fs,
                    Brush& brush,
                    SCObject& strokeLayer,
                    glm::vec2& lastPlacedPos,
                    bool& lastPosValid,
                    bool mouseWasDown)
{
    if (fs.mouseDown && !mouseWasDown) {
        auto verts = brush.generateVertices(fs.worldPos);
        strokeLayer.addShape(verts);
        lastPlacedPos = fs.worldPos;
        lastPosValid  = true;
    }
    else if (fs.mouseDown && mouseWasDown) {
        if (lastPosValid) {
            float dist = glm::distance(fs.worldPos, lastPlacedPos);
            if (dist >= brush.spacing) {
                auto verts = brush.generateVertices(fs.worldPos);
                strokeLayer.addShape(verts);
                lastPlacedPos = fs.worldPos;
            }
        } else {
            auto verts = brush.generateVertices(fs.worldPos);
            strokeLayer.addShape(verts);
            lastPlacedPos = fs.worldPos;
            lastPosValid  = true;
        }
    }
    else if (!fs.mouseDown && mouseWasDown) {
        lastPosValid = false;
    }
}

// -------------------------------------------------------------
// main
// -------------------------------------------------------------
int main() {
    if (!glfwInit()) return -1;

    Window win(1280, 720, "SCED Paint Test");
    GLFWwindow* window = win.getNativeWindow();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    Renderer2D renderer;
    Shader shader = Shader::fromFiles("Shader/config/flat.vert", "Shader/config/flat.frag");

    Input input;
    input.initialize(window);

    // ---------------------------------------------------------
    // Layers (timeline of frames)
    // ---------------------------------------------------------
    LayerStack layers(&renderer);      // starts with one empty SCObject layer

    // ---------------------------------------------------------
    // Background
    // ---------------------------------------------------------
    SCObject background(&renderer);
    RectangleShape newFullScreen(
        SCArch::Rect(10.0, 10.0, {-2.0f, 2.0f}, SColor::normalizeColor(245, 245, 245))
    );
    background.addShape(newFullScreen);

    // ---------------------------------------------------------
    // Frame border object
    // ---------------------------------------------------------
    SCObject frameObj(&renderer);
    RectangleShape frameRect(
        SCArch::Rect(1.2, 6.0, {-1.9f, 1.0f}, SColor::normalizeColor(127, 127, 127))
    );
    RectangleShape upprFrameRect(
        SCArch::Rect(8.0, .2, {-1.9f, 1.0f}, SColor::normalizeColor(127, 127, 127))
    );
    RectangleShape lowerFrameRect(
        SCArch::Rect(8.0, .2, {-1.9f, -1.0f}, SColor::normalizeColor(127, 127, 127))
    );
    RectangleShape leftFrame(
        SCArch::Rect(.3, 8.0, {1.8f, 1.0f}, SColor::normalizeColor(127, 127, 127))
    );

    auto frameHandle      = frameObj.addShape(frameRect);
    auto upprFrameHandle  = frameObj.addShape(upprFrameRect);
    auto lowerFrameHandle = frameObj.addShape(lowerFrameRect);
    auto leftFrameHandle  = frameObj.addShape(leftFrame);

    // Treat the main frame rectangle as a "button" for hit-testing, no callback needed.
    SCButton frameBtn(&frameObj, frameHandle, &renderer);

    // ---------------------------------------------------------
    // Brush
    // ---------------------------------------------------------
    Brush brush;
    brush.radius   = 0.01f;
    brush.spacing  = 0.00001f;
    brush.color    = SColor::normalizeColor(0, 0, 0);
    brush.segments = 36;

    bool      mouseWasDown  = false;
    glm::vec2 lastPlacedPos = glm::vec2(0.0f, 0.0f);
    bool      lastPosValid  = false;

    // ---------------------------------------------------------
    // UI Manager
    // ---------------------------------------------------------
    UIManager ui;
    ui.addButton(&frameBtn);

    // ---------------------------------------------------------
    // Color + size buttons (all as before, now registered with UIManager)
    // ---------------------------------------------------------

    // Red
    SCObject redButton(&renderer);
    auto redBtnHandleShape = redButton.addShape(
        CircleShape({-1.7f, .9f}, .05f, 64, SColor::normalizeColor(255, 163, 163))
    );
    SCButton redBtn(&redButton, redBtnHandleShape, &renderer);
    bool toggledRed = false;
    redBtn.setCallback([&](){
        toggledRed = !toggledRed;
        if (toggledRed) {
            brush.color = glm::vec3(1.0f, 0.2f, 0.2f);
            redButton.setShapeColor(redBtnHandleShape, glm::vec3(1.0f, 0.2f, 0.2f));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            redButton.setShapeColor(redBtnHandleShape, SColor::normalizeColor(255, 163, 163));
        }
        std::cout << "red button clicked\n";
    });
    ui.addButton(&redBtn);

    // Blue
    SCObject blueButton(&renderer);
    auto blueBtnHandleShape = blueButton.addShape(
        CircleShape({-1.56f, .9f}, .05f, 64, SColor::normalizeColor(163, 163, 255))
    );
    SCButton blueBtn(&blueButton, blueBtnHandleShape, &renderer);
    bool toggledBlue = false;
    blueBtn.setCallback([&](){
        toggledBlue = !toggledBlue;
        if (toggledBlue) {
            brush.color = SColor::normalizeColor(0, 0, 255);
            blueButton.setShapeColor(blueBtnHandleShape, SColor::normalizeColor(0, 0, 255));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            blueButton.setShapeColor(blueBtnHandleShape, SColor::normalizeColor(163, 163, 255));
        }
        std::cout << "blue button clicked\n";
    });
    ui.addButton(&blueBtn);

    // Yellow
    SCObject yellowButton(&renderer);
    auto yellowBtnHandleShape = yellowButton.addShape(
        CircleShape({-1.7f, 0.76f}, .05f, 64, SColor::normalizeColor(220, 220, 163))
    );
    SCButton yellowBtn(&yellowButton, yellowBtnHandleShape, &renderer);
    bool toggledYellow = false;
    yellowBtn.setCallback([&](){
        toggledYellow = !toggledYellow;
        if (toggledYellow) {
            brush.color = SColor::normalizeColor(255, 255, 0);
            yellowButton.setShapeColor(yellowBtnHandleShape, SColor::normalizeColor(255, 255, 0));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            yellowButton.setShapeColor(yellowBtnHandleShape, SColor::normalizeColor(200, 200, 163));
        }
    });
    ui.addButton(&yellowBtn);

    // Green
    SCObject greenButton(&renderer);
    auto greenBtnHandleShape = greenButton.addShape(
        CircleShape({-1.56f, 0.76f}, .05f, 64, SColor::normalizeColor(163, 255, 163))
    );
    SCButton greenBtn(&greenButton, greenBtnHandleShape, &renderer);
    bool toggledGreen = false;
    greenBtn.setCallback([&](){
        toggledGreen = !toggledGreen;
        if (toggledGreen) {
            brush.color = SColor::normalizeColor(0, 255, 0);
            greenButton.setShapeColor(greenBtnHandleShape, SColor::normalizeColor(0, 255, 0));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            greenButton.setShapeColor(greenBtnHandleShape, SColor::normalizeColor(163, 255, 163));
        }
    });
    ui.addButton(&greenBtn);

    // Purple
    SCObject purpleButton(&renderer);
    auto purpleBtnHandleShape = purpleButton.addShape(
        CircleShape({-1.7f, 0.62f}, .05f, 64, SColor::normalizeColor(200, 100, 200))
    );
    SCButton purpleBtn(&purpleButton, purpleBtnHandleShape, &renderer);
    bool toggledPurple = false;
    purpleBtn.setCallback([&](){
        toggledPurple = !toggledPurple;
        if (toggledPurple) {
            brush.color = SColor::normalizeColor(128, 0, 128);
            purpleButton.setShapeColor(purpleBtnHandleShape, SColor::normalizeColor(80, 0, 80));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            purpleButton.setShapeColor(purpleBtnHandleShape, SColor::normalizeColor(200, 100, 200));
        }
    });
    ui.addButton(&purpleBtn);

    // Orange
    SCObject orangeButton(&renderer);
    auto orangeBtnHandleShape = orangeButton.addShape(
        CircleShape({-1.56f, 0.62f}, .05f, 64, SColor::normalizeColor(255, 177, 102))
    );
    SCButton orangeBtn(&orangeButton, orangeBtnHandleShape, &renderer);
    bool toggledOrange = false;
    orangeBtn.setCallback([&](){
        toggledOrange = !toggledOrange;
        if (toggledOrange) {
            brush.color = SColor::normalizeColor(255, 127, 39);
            orangeButton.setShapeColor(orangeBtnHandleShape, SColor::normalizeColor(255, 127, 39));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            orangeButton.setShapeColor(orangeBtnHandleShape, SColor::normalizeColor(255, 177, 102));
        }
    });
    ui.addButton(&orangeBtn);

    // Brown
    SCObject brownButton(&renderer);
    auto brownBtnHandleShape = brownButton.addShape(
        CircleShape({-1.7f, 0.48f}, .05f, 64, SColor::normalizeColor(185, 150, 124))
    );
    SCButton brownBtn(&brownButton, brownBtnHandleShape, &renderer);
    bool toggledBrown = false;
    brownBtn.setCallback([&](){
        toggledBrown = !toggledBrown;
        if (toggledBrown) {
            brush.color = SColor::normalizeColor(185, 122, 87);
            brownButton.setShapeColor(brownBtnHandleShape, SColor::normalizeColor(185, 122, 87));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            brownButton.setShapeColor(brownBtnHandleShape, SColor::normalizeColor(185, 150, 124));
        }
    });
    ui.addButton(&brownBtn);

    // White
    SCObject whiteButton(&renderer);
    auto whiteBtnHandleShape = whiteButton.addShape(
        CircleShape({-1.56f, 0.48f}, .05f, 64, SColor::normalizeColor(230, 230, 230))
    );
    SCButton whiteBtn(&whiteButton, whiteBtnHandleShape, &renderer);
    bool toggledWhite = false;
    whiteBtn.setCallback([&](){
        toggledWhite = !toggledWhite;
        if (toggledWhite) {
            brush.color = SColor::normalizeColor(255, 255, 255);
            whiteButton.setShapeColor(whiteBtnHandleShape, SColor::normalizeColor(255, 255, 255));
        } else {
            brush.color = SColor::normalizeColor(0, 0, 0);
            whiteButton.setShapeColor(whiteBtnHandleShape, SColor::normalizeColor(230, 230, 230));
        }
    });
    ui.addButton(&whiteBtn);

    // ---------------------------------------------------------
    // Brush size UP arrow button
    // ---------------------------------------------------------
    SCObject upArrowButton(&renderer);
    glm::vec2 upCenter = { -1.4f, 0.9f };

    CircleShape upBgCircleLocal(
        {0.0f, 0.0f},
        0.08f,
        64,
        SColor::normalizeColor(255, 255, 255)
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

    ShapeHandle upBgHandle = upArrowButton.addShape(upBgVerts);
    ShapeHandle arrowUpHandle = upArrowButton.addShape(upArrow);

    SCButton arrowUpBtn(&upArrowButton, upBgHandle, &renderer);
    arrowUpBtn.setCallback([&]() {
        int percentage_to_add = 10;
        double ten_percent = (percentage_to_add / 100.0) * brush.radius;
        brush.radius += ten_percent;

        upArrowButton.setShapeColor(upBgHandle, SColor::normalizeColor(255, 255, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        upArrowButton.setShapeColor(upBgHandle, SColor::normalizeColor(255, 255, 255));

        std::cout << "Brush radius: " << brush.radius << std::endl;
    });
    ui.addButton(&arrowUpBtn);

    // ---------------------------------------------------------
    // Brush size DOWN arrow button
    // ---------------------------------------------------------
    SCObject dwnArrowButton(&renderer);
    glm::vec2 dwnCenter = { -1.4f, 0.7f };

    CircleShape bgCircleLocal(
        {0.0f, 0.0f},
        0.08f,
        64,
        SColor::normalizeColor(255, 255, 255)
    );

    auto bgVerts = bgCircleLocal.generateVertices();
    for (auto& v : bgVerts) {
        v.pos += dwnCenter;
    }

    std::vector<Vertex2D> downArrow = {
        { glm::vec2(-0.03f, 0.02f), glm::vec3(0,0,0) },
        { glm::vec2( 0.03f, 0.02f), glm::vec3(0,0,0) },
        { glm::vec2( 0.00f,-0.04f), glm::vec3(0,0,0) }
    };

    for (auto& v : downArrow) {
        v.pos += dwnCenter;
    }

    ShapeHandle bgH    = dwnArrowButton.addShape(bgVerts);
    ShapeHandle arrowH = dwnArrowButton.addShape(downArrow);

    SCButton arrowDownBtn(&dwnArrowButton, bgH, &renderer);
    arrowDownBtn.setCallback([&](){
        int percentage_to_subtract = 10;
        double ten_percent = (percentage_to_subtract / 100.0) * brush.radius;
        brush.radius -= ten_percent;

        dwnArrowButton.setShapeColor(bgH, SColor::normalizeColor(255, 255, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        dwnArrowButton.setShapeColor(bgH, SColor::normalizeColor(255, 255, 255));

        std::cout << "Brush radius: " << brush.radius << std::endl;
    });
    ui.addButton(&arrowDownBtn);

    // ---------------------------------------------------------
    // Key edge-state for frame navigation and onion
    // ---------------------------------------------------------
    bool prevRight = false;
    bool prevLeft  = false;
    bool prevO     = false;

    // ---------------------------------------------------------
    // Main loop
    // ---------------------------------------------------------
    while (!win.shouldClose()) {
        glfwPollEvents();

        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        if (height == 0) height = 1;
        float aspect = float(width) / float(height);

        glViewport(0, 0, width, height);
        glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Snapshot of input for this frame (world pos, mouse, etc.)
        FrameState fi = updateFrameInput(input, window, width, height, aspect, mouseWasDown);

        // Frame navigation + onion toggle (edge triggered)
        int rightState = glfwGetKey(window, GLFW_KEY_RIGHT);
        int leftState  = glfwGetKey(window, GLFW_KEY_LEFT);
        int oState     = glfwGetKey(window, GLFW_KEY_O);

        bool justRight = (rightState == GLFW_PRESS && !prevRight);
        bool justLeft  = (leftState  == GLFW_PRESS && !prevLeft);
        bool justO     = (oState     == GLFW_PRESS && !prevO);

        if (justRight) {
            layers.next();
        }
        if (justLeft) {
            layers.prev();
        }
        if (justO) {
            layers.toggleOnion();
        }

        prevRight = (rightState == GLFW_PRESS);
        prevLeft  = (leftState  == GLFW_PRESS);
        prevO     = (oState     == GLFW_PRESS);

        // Update all UI buttons with this frame's input
        ui.updateAll(fi, mouseWasDown);

        // Determine if cursor is over any UI element
        bool overUI = ui.anyContains(fi.worldPos);

        // Paint only if not over UI
        if (!overUI) {
            handlePainting(fi, brush, layers.current(), lastPlacedPos, lastPosValid, mouseWasDown);
        }

        mouseWasDown = fi.mouseDown;

        // -----------------------------------------------------
        // Draw
        // -----------------------------------------------------
        glm::mat4 vp = glm::ortho(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);

        background.draw(shader, vp);

        // Onion layer (previous frame) if enabled
        if (const SCObject* onion = layers.onion()) {
            for (ShapeHandle h : onion->getShapeHandles()) {
                renderer.setOverrideColor(h, SColor::normalizeColor(200, 200, 200));
            }
        
            onion->draw(shader, vp);
        
            for (ShapeHandle h : onion->getShapeHandles()) {
                renderer.clearOverrideColor(h);
            }
        }

        // Current drawing layer
        layers.current().draw(shader, vp);

        // Frame border
        frameObj.draw(shader, vp);

        // All UI buttons
        ui.drawAll(shader, vp);

        glfwSwapBuffers(window);
        input.endFrame();
    }

    glfwTerminate();
    return 0;
}
