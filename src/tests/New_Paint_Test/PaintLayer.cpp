#include "PaintLayer.h"
#include <iostream>

// ---------- Brush generation ----------
std::vector<Vertex2D> PaintLayer::Brush::generate(const glm::vec2& pos) const
{
    CircleShape circ(pos, radius, segments, color);
    return circ.generateVertices();
}

// ---------- ctor ----------
PaintLayer::PaintLayer(Renderer2D* r, Shader* s, Input* in, Window* w)
    : renderer(r)
    , shader(s)
    , input(in)
    , window(w)
    , background(r)
    , redBtnObj(r)
    , blueBtnObj(r)
{
    // Brush init
    brush.radius   = 0.01f;
    brush.spacing  = 0.00001f;
    brush.color    = SColor::normalizeColor(0, 0, 0);
    brush.segments = 36;

    lastPosValid  = false;
    mouseWasDown  = false;

    // ---------- Background ----------
    {
        // Same idea as your newBackground
        RectangleShape bgRect(
            SCArch::Rect(10.0, 10.0, {-2.0f, 2.0f}, SColor::normalizeColor(245, 245, 245))
        );
        background.addShape(bgRect);
    }

    // ---------- Frames ----------
    frames.emplace_back(renderer); // frame 0
    currentFrame = 0;

    // ---------- Red button ----------
    {
        CircleShape redCircle(
            {-1.7f, 0.9f},
            0.05f,
            64,
            SColor::normalizeColor(255, 163, 163)
        );
        ShapeHandle redHandle = redBtnObj.addShape(redCircle);

        redBtn = std::make_unique<SCButton>(&redBtnObj, redHandle, renderer);

        redBtn->setCallback([this, redHandle]() {
            brush.color = glm::vec3(1.0f, 0.2f, 0.2f);
            redBtnObj.setShapeColor(redHandle, glm::vec3(1.0f, 0.2f, 0.2f));
            std::cout << "red button clicked\n";
        });
    }

    // ---------- Blue button ----------
    {
        CircleShape blueCircle(
            {-1.56f, 0.9f},
            0.05f,
            64,
            SColor::normalizeColor(163, 163, 255)
        );
        ShapeHandle blueHandle = blueBtnObj.addShape(blueCircle);

        blueBtn = std::make_unique<SCButton>(&blueBtnObj, blueHandle, renderer);

        blueBtn->setCallback([this, blueHandle]() {
            brush.color = SColor::normalizeColor(0, 0, 255);
            blueBtnObj.setShapeColor(blueHandle, SColor::normalizeColor(0, 0, 255));
            std::cout << "blue button clicked\n";
        });
    }
}

// ---------- painting core ----------
void PaintLayer::handlePaint(const FrameState& fs)
{
    SCObject& active = frames[currentFrame];

    if (fs.mouseDown && !mouseWasDown) {
        // first click
        auto verts = brush.generate(fs.worldPos);
        active.addShape(verts);
        lastPlacedPos = fs.worldPos;
        lastPosValid  = true;
    }
    else if (fs.mouseDown && mouseWasDown) {
        // drag
        if (lastPosValid) {
            float dist = glm::distance(fs.worldPos, lastPlacedPos);
            if (dist >= brush.spacing) {
                auto verts = brush.generate(fs.worldPos);
                active.addShape(verts);
                lastPlacedPos = fs.worldPos;
            }
        } else {
            auto verts = brush.generate(fs.worldPos);
            active.addShape(verts);
            lastPlacedPos = fs.worldPos;
            lastPosValid  = true;
        }
    }
    else if (!fs.mouseDown && mouseWasDown) {
        // released
        lastPosValid = false;
    }

    mouseWasDown = fs.mouseDown;
}

// ---------- per-frame update ----------
void PaintLayer::onUpdate(const FrameState& fs)
{
    // Let buttons process click events
    if (redBtn)  redBtn->update(fs, mouseWasDown);
    if (blueBtn) blueBtn->update(fs, mouseWasDown);

    bool hovering =
        (redBtn  && redBtn->contains(fs.worldPos)) ||
        (blueBtn && blueBtn->contains(fs.worldPos));

    // If cursor is not over any button, paint
    if (!hovering) {
        handlePaint(fs);
    }

    // Optionally set fs.overButton in Application if you want
    // but here we just use local "hovering"
}

// ---------- render ----------
void PaintLayer::onRender(const glm::mat4& vp)
{
    // Background
    background.draw(*shader, vp);

    // Current frame strokes
    frames[currentFrame].draw(*shader, vp);

    // UI
    if (redBtn)  redBtn->draw(*shader, vp);
    if (blueBtn) blueBtn->draw(*shader, vp);
}
