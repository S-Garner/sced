#pragma once
#include "IShape2D.hpp"
#include "Shapes.hpp"   // uses Shapes::makeEllipse

class EllipseShape : public IShape2D {
public:
    EllipseShape(glm::vec2 center, glm::vec2 radii, int segments, glm::vec3 color)
            : c(center), radii(radii), segs(segments), col(color) {}

    std::vector<Vertex2D> generateVertices() const override {
        return Shapes::makeEllipse(c, radii, segs, col);
    }

private:
    glm::vec2 c;
    glm::vec2 radii;    // {rx, ry}
    int       segs;
    glm::vec3 col;
};
