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
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "SCED Paint Test", nullptr, nullptr);
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
        background.addShape(fullScreenRect);
    }

    // Stroke container
    SCObject stroke(&renderer);

    // Brush
    Brush brush;
    brush.radius   = 0.1f;
    brush.spacing  = 0.00001f;
    brush.color    = glm::vec3(240/255.f, 240/255.f, 255/255.f);
    brush.segments = 36;

    bool      mouseWasDown  = false;
    glm::vec2 lastPlacedPos = glm::vec2(0.0f, 0.0f);
    bool      lastPosValid  = false;

    // ******* Red Button **********
    SCObject redButton(&renderer);

    auto rectShape = CircleShape({-1.25f, .9f}, .05f, 64, SColor::normalizeColor(255, 163, 163));

    auto redBtnHandle = redButton.addShape(rectShape);

    // Button wrapper
    SCButton redBtn(&redButton, redBtnHandle, &renderer);

    bool toggled = false;

    // On-click callback
    redBtn.setCallback([&](){
        toggled = !toggled;

        if (toggled) { // if button is pressed
        brush.color = glm::vec3(1.0f, 0.2f, 0.2f);

        redButton.setShapeColor(
            redBtnHandle,                // already stored
            glm::vec3(1.0f, 0.2f, 0.2f)
        );
        } else if (!toggled){ // Button is not pressed
            brush.color = glm::vec3(240/255.f, 240/255.f, 255/255.f);

            redButton.setShapeColor(
                redBtnHandle,                // already stored
                SColor::normalizeColor(255, 163, 163));
        }

        std::cout << "red button clicked\n";
    });

    // ******* Red Button **********

    // ******* Blue Button **********

    SCObject blueButton(&renderer);

    auto blueCirc = CircleShape({-1.10f, .9f}, .05f, 64, SColor::normalizeColor(163, 163, 255));

    auto blueBtnHandle = blueButton.addShape(blueCirc);

    // Button wrapper
    SCButton blueBtn(&blueButton, blueBtnHandle, &renderer);

    bool toggledBlue = false;

    // On-click callback
    blueBtn.setCallback([&](){
        toggledBlue = !toggledBlue;

        if (toggledBlue) { // if button is pressed
        //brush.color = glm::vec3(1.0f, 0.2f, 0.2f);
            brush.color = SColor::normalizeColor(0, 0, 255);

            blueButton.setShapeColor(
                blueBtnHandle,                // already stored
                SColor::normalizeColor(0, 0, 255)
        );
        } else if (!toggledBlue){ // Button is not pressed
            brush.color = SColor::normalizeColor(255, 255, 255);

            blueButton.setShapeColor(
                blueBtnHandle,                // already stored
                SColor::normalizeColor(163, 163, 255));
        }

        std::cout << "blue button clicked\n";
    });

    // ******* Blue Button **********

// *******************************************************************
SCObject arrowButton(&renderer);

// -------------------------------------------------------------
// 1) World-space center for the whole button
glm::vec2 center = { -1.0f, 0.7f };

// -------------------------------------------------------------
// 2) Background circle — generate in world-space
CircleShape bgCircleLocal(
    {0.0f, 0.0f},      // local
    0.08f,
    64,
    SColor::normalizeColor(255,255,255)
);

auto bgVerts = bgCircleLocal.generateVertices();
for (auto& v : bgVerts) {
    v.pos += center;         // shift to world-space
}

// -------------------------------------------------------------
// 3) Arrow triangle — also in world-space
std::vector<Vertex2D> arrowLocal = {
    { glm::vec2( 0.00f,  0.04f), glm::vec3(0,0,0) },
    { glm::vec2( 0.03f, -0.02f), glm::vec3(0,0,0) },
    { glm::vec2(-0.03f, -0.02f), glm::vec3(0,0,0) },
};

// Move arrow into world space
for (auto& v : arrowLocal) {
    v.pos += center;
}

ShapeHandle arrowH = arrowButton.addShape(arrowLocal);
ShapeHandle bgH = arrowButton.addShape(bgVerts);

// -------------------------------------------------------------
// 4) Wrap SCButton
SCButton arrowBtn(&arrowButton, bgH, &renderer);

// -------------------------------------------------------------
// 5) Callback
arrowBtn.setCallback([&](){
    std::cout << "Arrow button clicked\n";

    int percentage_to_subtract = 10;
    double ten_percent = (percentage_to_subtract / 100.0) * brush.radius;
    brush.radius -= ten_percent;

    arrowButton.setShapeColor(bgH, SColor::normalizeColor(255,255,0)); 
});
// *******************************************************************


    int width = 0, height = 0;

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

        // First let the button see the old mouse state
        redBtn.update(fs, mouseWasDown);
        blueBtn.update(fs, mouseWasDown);
        arrowBtn.update(fs, mouseWasDown);

        // Ignore painting when mouse is over the button
        if (!redBtn.contains(fs.worldPos) && !blueBtn.contains(fs.worldPos) && !arrowBtn.contains(fs.worldPos)) {
            handlePainting(fs, brush, stroke, lastPlacedPos, lastPosValid, mouseWasDown);
        }

        // Now advance mouseWasDown to this frame
        mouseWasDown = fs.mouseDown;

        glm::mat4 vp = glm::ortho(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);

        background.draw(shader, vp);
        stroke.draw(shader, vp);

        redBtn.draw(shader, vp);
        blueBtn.draw(shader, vp);
        arrowBtn.draw(shader, vp);

        glfwSwapBuffers(window);
        input.endFrame();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
