//
// Created by sethg on 10/4/2025.
//

#ifndef SCED_TRIANGLE_H
#define SCED_TRIANGLE_H
#include "Vertex.h"


class Triangle
{
private:
    std::vector<Vertex> vertices;

public:
    Triangle() = default;

    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
        : vertices{v1, v2, v3} {}

    Vertex& operator[](size_t index)
    {
        return vertices.at(index);
    }

    const Vertex& operator[](size_t index) const
    {
        return vertices.at(index);
    }

    std::vector<Vertex>& getVertices()
    {
        return vertices;
    }

    const std::vector<Vertex>& getVertices() const
    {
        return vertices;
    }

    void setVertex(size_t index, const Vertex& vert)
    {
        if (index < vertices.size())
            vertices[index] = vert;
    }

    void addVertex(const Vertex& vert)
    {
        if (vertices.size() < 3)
            vertices.push_back(vert);
    }

    // --- Helper: get raw float data (for OpenGL buffers)
    std::vector<float> getPositionData() const
    {
        std::vector<float> all;
        for (const auto& v : vertices)
        {
            if (v.has("position"))
            {
                const auto& pos = v.get("position").data;
                all.insert(all.end(), pos.begin(), pos.end());
            }
        }
        return all;
    }
};


#endif //SCED_TRIANGLE_H