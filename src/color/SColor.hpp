#pragma once
#include <glm/glm.hpp>

namespace SColor {
    inline float normalizeColorComponent(int component) {
        return static_cast<float>(component) / 255.0f;
    }

    inline glm::vec3 normalizeColor(int r, int g, int b) {
        return glm::vec3(
            normalizeColorComponent(r),
            normalizeColorComponent(g),
            normalizeColorComponent(b)
        );
    }
}