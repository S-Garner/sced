#pragma once
#include "IShape2D.hpp"
#include "Shapes.hpp"   // uses Shapes::makeRegularPolygon

class RegularPolygonShape : public IShape2D {
public:
    RegularPolygonShape(glm::vec2 center, float radius, int sides, glm::vec3 color)
            : c(center), r(radius), n(sides), col(color) {}

    std::vector<Vertex2D> generateVertices() const override {
        return Shapes::makeRegularPolygon(c, r, n, col);
    }

private:
    glm::vec2 c;
    float     r;
    int       n;
    glm::vec3 col;
};
