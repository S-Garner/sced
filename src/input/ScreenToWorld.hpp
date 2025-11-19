#pragma once
#include <glm/glm.hpp>

inline glm::vec2 screenToWorld(double mx, double my, int width, int height, float aspect) {
    float x = float(mx) / float(width);
    float y = float(my) / float(height);
    x = x * 2.0f - 1.0f;
    y = -y * 2.0f + 1.0f;
    return { x * aspect, y };
}