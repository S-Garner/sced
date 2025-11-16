#pragma once

#include <functional>

#include "../../objects/SCObject.hpp"
#include "../../Renderer/Renderer2D.hpp"
#include "../FrameState.hpp"

class SCButton {
private:
    SCObject*    target;
    Renderer2D*  renderer;
    ShapeHandle  handle;

    glm::vec2    boxMin;
    glm::vec2    boxMax;

    std::function<void()> callback;

public:
    SCButton(SCObject* obj, ShapeHandle handle, Renderer2D* renderer);

    // frameState: snapshot of mouse for this frame
    // mouseWasDown: previous frame's mouseDown
    void update(const FrameState& frameState, bool mouseWasDown);

    void draw(const Shader& s, const glm::mat4& vp);

    void setCallback(std::function<void()> fn);

    // hit test in world space
    bool contains(const glm::vec2& p) const {
        return p.x >= boxMin.x && p.x <= boxMax.x &&
               p.y >= boxMin.y && p.y <= boxMax.y;
    }

    glm::vec2 getBoxMin() const { return boxMin; }
    glm::vec2 getBoxMax() const { return boxMax; }

private:
    void computeHitBox();
};
