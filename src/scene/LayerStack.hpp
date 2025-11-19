#pragma once
#include <vector>
#include "../objects/SCObject.hpp"

class LayerStack {
public:
    explicit LayerStack(Renderer2D* renderer) : renderer(renderer) {
        layers.emplace_back(renderer); // start with one empty layer
    }

    SCObject& current() { return layers[currentIndex]; }
    const SCObject* onion() const {
        if (!onionEnabled || currentIndex == 0) return nullptr;
        return &layers[currentIndex - 1];
    }

    void next() {
        if (currentIndex == layers.size() - 1) layers.emplace_back(renderer);
        ++currentIndex;
    }

    void prev() { if (currentIndex > 0) --currentIndex; }

    void toggleOnion() { onionEnabled = !onionEnabled; }
    bool onionOn() const { return onionEnabled; }

    const std::vector<SCObject>& all() const { return layers; }

private:
    Renderer2D* renderer;
    std::vector<SCObject> layers;
    size_t currentIndex{0};
    bool onionEnabled{false};
};
