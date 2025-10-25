#ifndef SCED_ISHAPE2D_HPP
#define SCED_ISHAPE2D_HPP

#include <vector>
#include <glm/glm.hpp>
#include "Vertex2D.hpp"

// This is the abstract base class (interface)
struct IShape2D {
    virtual ~IShape2D() = default;

    // Generate triangle-list vertices in model/local space.
    virtual std::vector<Vertex2D> generateVertices() const = 0;
};

#endif // SCED_ISHAPE2D_HPP
