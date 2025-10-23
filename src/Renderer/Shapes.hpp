#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Vertex2D.hpp"
#include <cmath>

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
    inline std::vector<Vertex2D> makeCircle(glm::vec2 center, float radius, int segments, const glm::vec3& color) {
     std::vector<Vertex2D> out;
        if (radius <= 0.0f || segments < 3) return out;

        out.reserve(static_cast<size_t>(segments) *3);
        const float twoPi = 6.2831853071795864769f;

        Vertex2D vCenter{ center, color};
        glm::vec2 p0 = center +radius * glm::vec2(std::cos(0.0f),std::sin(0.0f));
        Vertex2D vPrev{ p0, color};

        for (int i =1; i <= segments; ++i) {
        float a = (twoPi * i) / static_cast<float>(segments);
        glm::vec2 p = center + radius * glm::vec2(std::cos(a), std::sin(a));
        Vertex2D vCurr{p, color};

        out.push_back(vCenter);
        out.push_back(vPrev);
        out.push_back(vCurr);

        vPrev = vCurr;
        }

        return out;

    }

    inline std::vector<Vertex2D> makeEllipse(glm::vec2 center, glm::vec2 radii, int segments, const glm::vec3& color) {
        std::vector<Vertex2D> out;
        if (radii.x <= 0.0f || radii.y <= 0.0f || segments < 3) return out;

        out.reserve(static_cast<size_t>(segments) * 3);
        const float twoPi = 6.2831853071795864769f;

        auto ept = [&](float a) {
            return center + glm::vec2(radii.x * std::cos(a), radii.y * std::sin(a));
        };

        Vertex2D vCenter{ center, color };
        Vertex2D vPrev{ ept(0.0f), color };

        for (int i = 1; i <= segments; ++i) {
            float a = (twoPi * i) / static_cast<float>(segments);
            Vertex2D vCurr{ ept(a), color };

            out.push_back(vCenter);
            out.push_back(vPrev);
            out.push_back(vCurr);

            vPrev = vCurr;
        }

        return out;
    }
    inline std::vector<Vertex2D> makeRegularPolygon(glm::vec2 center, float radius, int sides, const glm::vec3& color) {
        if (sides < 3) return {};
        return makeCircle(center, radius, sides, color);
    }
}