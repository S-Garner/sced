#pragma once
#include <glm/glm.hpp>

struct ShapeRecord {
    uint32_t id;
    int offset;
    int count;
    glm::mat4 model{1.0f};
    bool useOverride=false;
    glm::vec3 overrideColor{1, 1, 1};
};