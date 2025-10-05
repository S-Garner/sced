//
// Created by sethg on 10/4/2025.
//

#ifndef SCED_MESH_H
#define SCED_MESH_H
#include "Triangle.h"

class Mesh
{
private:
    std::vector<Triangle> triangles;

public:
    Triangle& operator[](size_t index) { return triangles.at(index); }
    const Triangle& operator[](size_t index) const { return triangles.at(index); }

    std::vector<Triangle>& getTriangles() { return triangles; }
    const std::vector<Triangle>& getTriangles() const { return triangles; }

    void addTriangle(const Triangle& tri) { triangles.push_back(tri); }

    std::vector<Vertex*> getAllVertices()
    {
        std::vector<Vertex*> verts;
        for (auto& tri : triangles)
            for (auto& v : tri.getVertices())
                verts.push_back(&v);
        return verts;
    }

    // --- Flattened raw data for rendering
    std::vector<float> getPositionData() const
    {
        std::vector<float> data;
        for (const auto& tri : triangles)
        {
            auto triData = tri.getPositionData();
            data.insert(data.end(), triData.begin(), triData.end());
        }
        return data;
    }
};

#endif //SCED_MESH_H