#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Vertex2D.hpp"

namespace Shapes {
    std::vector<Vertex2D> makeRectangle (glm::vec2 minXY, glm::vec2 maxXY, const glm::vec3& color) {
        const glm::vec2 bottomLeft = minXY;
        const glm::vec2 bottomRight = {maxXY.x, minXY.y};
        const glm::vec2 topRight = maxXY;
        const glm::vec2 topLeft = {minXY.x, maxXY.y};

        return {
            {bottomLeft, color}, {bottomRight, color}, {topRight, color},
            {bottomLeft, color}, {topRight, color}, {topLeft, color}
        };
    }

    std::vector<Vertex2D> makeTriangle(glm::vec2 vec1, glm::vec2 vec2, glm::vec2 vec3, const glm::vec3& color) {
        return { {vec1, color}, {vec2, color}, {vec3, color} };
    }
}