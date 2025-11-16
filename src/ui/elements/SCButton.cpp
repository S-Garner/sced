#include "SCButton.hpp"

SCButton::SCButton(SCObject* obj, ShapeHandle handle, Renderer2D* renderer)
    : target(obj), renderer(renderer), handle(handle)
{
    computeHitBox();
}

void SCButton::update(const FrameState& frameState, bool mouseWasDown) {
    computeHitBox();

    bool inside =
        frameState.worldPos.x >= boxMin.x &&
        frameState.worldPos.x <= boxMax.x &&
        frameState.worldPos.y >= boxMin.y &&
        frameState.worldPos.y <= boxMax.y;

    bool justPressed = frameState.mouseDown && !mouseWasDown;

    if (inside && justPressed && callback) {
        callback();
    }
}

void SCButton::draw(const Shader& s, const glm::mat4& vp) {
    target->draw(s, vp);
}

void SCButton::setCallback(std::function<void()> function) {
    callback = function;
}

void SCButton::computeHitBox() {
    const auto& cpu    = renderer->getCPUBuffer();
    const auto& shapes = renderer->getShapes();

    const ShapeRecord* r = nullptr;
    for (auto& s : shapes) {
        if (s.id == handle.id) {
            r = &s;
            break;
        }
    }

    if (!r) {
        return;
    }

    glm::vec2 minP( 9999.f );
    glm::vec2 maxP(-9999.f );

    for (int i = 0; i < r->count; ++i) {
        const auto& v = cpu[r->offset + i].pos;
        minP.x = std::min(minP.x, v.x);
        minP.y = std::min(minP.y, v.y);
        maxP.x = std::max(maxP.x, v.x);
        maxP.y = std::max(maxP.y, v.y);
    }

    boxMin = minP;
    boxMax = maxP;
}
