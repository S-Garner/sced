#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <random>
#include <iostream>

#include "../Renderer/Renderer2D.hpp"
#include "../Renderer/Shapes/RectangleShape.hpp"
#include "../Renderer/Shapes/CircleShape.hpp"
#include "../Renderer/Shader/Shader.hpp"
#include "../Renderer/Transform.hpp"
#include "../Renderer/Vertex2D.hpp"

#include "../objects/SCObject.hpp"
#include "../ui/elements/SCButton.hpp"
#include "../input/Input.h"

// -------------------------------------------------------------
// Utility
glm::vec2 screenToWorld(double mx, double my, int w, int h) {
    float x = float(mx) / float(w);
    float y = float(my) / float(h);
    x = x * 2.f - 1.f;
    y = -y * 2.f + 1.f;
    return {x, y};
}

FrameState fsUpdate(Input& in, GLFWwindow* win, int w, int h, float aspect) {
    FrameState fs{};
    double mx = in.getMouseX();
    double my = in.getMouseY();
    fs.worldPos = screenToWorld(mx, my, w, h);
    fs.worldPos.x *= aspect;
    fs.mouseDown =
        glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    return fs;
}


enum class GameState {
    ShowSequence,
    WaitInput
};

int main() {
    // -------------------------------------------------------------
    // Window + GL init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(800,600,"Simon Demo",nullptr,nullptr);
    if(!window) return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    Renderer2D renderer;
    Shader shader =
        Shader::fromFiles("Shader/config/flat.vert","Shader/config/flat.frag");

    Input input;
    input.initialize(window);

    // -------------------------------------------------------------
    // CREATE FOUR PADS
    struct Pad {
        SCObject obj;
        ShapeHandle h;
        glm::vec3 baseColor;
        glm::vec3 flashColor;
        SCButton* button;
    };

    Pad pads[4] = {
        { SCObject(&renderer), {}, {1.f,0.f,0.f}, {1.f,0.6f,0.6f}, nullptr },
        { SCObject(&renderer), {}, {0.f,1.f,0.f}, {0.6f,1.f,0.6f}, nullptr },
        { SCObject(&renderer), {}, {0.f,0.f,1.f}, {0.6f,0.6f,1.f}, nullptr },
        { SCObject(&renderer), {}, {1.f,1.f,0.f}, {1.f,1.f,0.5f}, nullptr }
    };

    // positions for 4 pads
    glm::vec2 pos[4] = {
        {-0.3f,  0.3f},
        { 0.3f,  0.3f},
        {-0.3f, -0.3f},
        { 0.3f, -0.3f}
    };

    for(int i=0;i<4;i++){
        CircleShape c({0,0},0.25f,64,pads[i].baseColor);
        auto verts = c.generateVertices();
        // shift to pos
        for(auto& v : verts) v.pos += pos[i];
        pads[i].h = pads[i].obj.addShape(verts);

        // wrap button
        pads[i].button = new SCButton(&pads[i].obj, pads[i].h, &renderer);
    }

    // -------------------------------------------------------------
    // SIMON GAME DATA
    std::vector<int> sequence;
    std::mt19937 rng{ std::random_device{}() };
    sequence.push_back(rng()%4);

    int playbackIndex = 0;
    double timer = 0.0;
    const double flashTime = 0.6;
    const double pauseTime = 0.3;
    bool flashing = false;

    GameState state = GameState::ShowSequence;
    int inputIndex = 0;

    bool mouseWasDown = false;
    int width=0, height=0;

    // -------------------------------------------------------------
    // MAIN LOOP
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwGetFramebufferSize(window,&width,&height);
        if(height==0) height=1;
        float aspect = float(width)/float(height);

        glViewport(0,0,width,height);
        glClearColor(0.08f,0.08f,0.1f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        FrameState fs = fsUpdate(input, window, width, height, aspect);
        fs.mouseJustPressed = (fs.mouseDown && !mouseWasDown);

        // BUTTON HIT UPDATE
        for(int i=0;i<4;i++)
            pads[i].button->update(fs, mouseWasDown);

        // ---------------------------------------------------------
        // GAME STATE MACHINE

        if(state == GameState::ShowSequence) {
            timer -= 1.0/60.0;

            if(!flashing) {
                // reset all pad colors
                for(int i=0;i<4;i++)
                    pads[i].obj.setShapeColor(pads[i].h, pads[i].baseColor);

                if(timer <= 0.0) {
                    if(playbackIndex >= (int)sequence.size()) {
                        state = GameState::WaitInput;
                        inputIndex = 0;
                    } else {
                        int pad = sequence[playbackIndex];
                        pads[pad].obj.setShapeColor(pads[pad].h,
                                                    pads[pad].flashColor);
                        flashing = true;
                        timer = flashTime;
                    }
                }
            } else {
                // currently flashing
                if(timer <= 0.0) {
                    int pad = sequence[playbackIndex];
                    pads[pad].obj.setShapeColor(pads[pad].h,
                                                pads[pad].baseColor);
                    flashing = false;
                    playbackIndex++;
                    timer = pauseTime;
                }
            }
        }

        else if(state == GameState::WaitInput) {
            // user clicks a pad
            for(int i=0;i<4;i++) {
                if(pads[i].button->contains(fs.worldPos)
                   && fs.mouseJustPressed) {

                    if(i == sequence[inputIndex]) {
                        inputIndex++;
                        if(inputIndex == (int)sequence.size()) {
                            // correct — extend sequence
                            sequence.push_back(rng()%4);
                            playbackIndex = 0;
                            flashing = false;
                            timer = pauseTime;
                            state = GameState::ShowSequence;
                        }
                    } else {
                        // wrong — reset the game
                        sequence.clear();
                        sequence.push_back(rng()%4);
                        playbackIndex = 0;
                        timer = pauseTime;
                        flashing = false;
                        state = GameState::ShowSequence;
                    }
                }
            }
        }

        mouseWasDown = fs.mouseDown;

        // ---------------------------------------------------------
        // DRAW
        glm::mat4 vp =
            glm::ortho(-aspect,aspect,-1.f,1.f,-1.f,1.f);

        for(int i=0;i<4;i++)
            pads[i].obj.draw(shader, vp);

        glfwSwapBuffers(window);
        input.endFrame();
    }

    glfwTerminate();
    return 0;
}
