#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "src/Application/Application.h"
#include "src/core/Window.h"
#include "src/input/Input.h"

#include "src/Renderer/Renderer2D.hpp"
#include "src/Renderer/Shader/Shader.hpp"
#include "src/Renderer/Vertex2D.hpp"
#include "src/Renderer/Shapes/CircleShape.hpp"
#include "src/Renderer/Shapes/RectangleShape.hpp"

#include "src/objects/SCObject.hpp"
#include "src/ui/elements/SCButton.hpp"
#include "src/ui/FrameState.hpp"
#include "src/color/SColor.hpp"
#include "src/converter/SCArch.cpp" // if you really need it; otherwise remove

class PaintLayer : public ToolLayer
{
public:
    PaintLayer(Renderer2D* r, Shader* s, Input* in, Window* w);

    void onUpdate(const FrameState& fs) override;
    void onRender(const glm::mat4& vp) override;

private:
    // Engine services (non-owning)
    Renderer2D* renderer;
    Shader*     shader;
    Input*      input;
    Window*     window;

    // Brush
    struct Brush {
        float radius;
        float spacing;
        glm::vec3 color;
        int   segments;

        std::vector<Vertex2D> generate(const glm::vec2& pos) const;
    } brush;

    bool      lastPosValid;
    glm::vec2 lastPlacedPos;
    bool      mouseWasDown;

    // Frames (each SCObject holds the stroke geometry for that frame)
    std::vector<SCObject> frames;
    int                   currentFrame;

    // Background
    SCObject background;

    // UI buttons
    SCObject                     redBtnObj;
    SCObject                     blueBtnObj;
    std::unique_ptr<SCButton>    redBtn;
    std::unique_ptr<SCButton>    blueBtn;

    // Internal helpers
    void handlePaint(const FrameState& fs);
};
