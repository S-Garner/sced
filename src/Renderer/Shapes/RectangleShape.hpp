#pragma once
#include "IShape2D.hpp"
#include "../Shapes.hpp"   // uses Shapes::makeRectangle

class RectangleShape : public IShape2D {
public:
    RectangleShape(glm::vec2 minXY, glm::vec2 maxXY, glm::vec3 color)
            : min(minXY), max(maxXY), col(color) {}

    std::vector<Vertex2D> generateVertices() const override {
        return Shapes::makeRectangle(min, max, col);
    }

private:
    glm::vec2 min, max;
    glm::vec3 col;
};
