#pragma once
#include <vector>
#include "elements/SCButton.hpp"
#include "FrameState.hpp"

class UIManager {
public:
    void addButton(SCButton* btn) { buttons.push_back(btn); }

    void updateAll(const FrameState& fi, bool prevMouseDown) {
        for (auto* b : buttons) b->update(fi, prevMouseDown);
    }

    bool anyContains(const glm::vec2& worldPos) const {
        for (auto* b : buttons) if (b->contains(worldPos)) return true;
        return false;
    }

    void drawAll(const Shader& shader, const glm::mat4& vp) const {
        for (auto* b : buttons) b->draw(shader, vp);
    }

private:
    std::vector<SCButton*> buttons;
};
