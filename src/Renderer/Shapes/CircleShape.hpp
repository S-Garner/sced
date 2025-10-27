#pragma once
#include "IShape2D.hpp"
#include "../Shapes.hpp"   // uses Shapes::makeCircle

class CircleShape : public IShape2D {
public:
    CircleShape(glm::vec2 center, float radius, int segments, glm::vec3 color)
            : c(center), r(radius), segs(segments), col(color) {}

    std::vector<Vertex2D> generateVertices() const override {
        return Shapes::makeCircle(c, r, segs, col);
    }

    int getCenterX() const { return c.x; }
    int getCenterY() const { return c.y; }

private:
    glm::vec2 c;
    float     r;
    int       segs;
    glm::vec3 col;
};
