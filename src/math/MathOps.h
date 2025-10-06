#ifndef SCED_MATHOPS_H
#define SCED_MATHOPS_H

#include <cmath>
#include <algorithm>
#include "../objects/Attribute.h"
#include "../objects/Triangle.h"
#include "../objects/Vertex.h"
#include "../objects/Mesh.h"

class Mesh;
class Triangle;

namespace Math
{
    // --- Helper: Round to 1 decimal place ---
    inline float round1(float value)
    {
        return std::round(value * 10.0f) / 10.0f;
    }

    // --- Attribute-level operations ---
    inline Attribute translate(const Attribute& attr, const Attribute& delta)
    {
        Attribute result(attr);
        for (size_t i = 0; i < std::min(attr.size(), delta.size()); ++i)
            result.data[i] = round1(attr.data[i] + delta.data[i]);
        return result;
    }

    inline Attribute scale(const Attribute& attr, float factor)
    {
        Attribute result(attr);
        for (auto& i : result.data)
            i = round1(i * factor);
        return result;
    }

    inline Attribute rotate2D(const Attribute& attr, float angle)
    {
        if (attr.size() < 2)
            return attr;

        float rad = angle * 3.14159265f / 180.0f;
        float x = attr.data[0];
        float y = attr.data[1];

        float rx = x * std::cos(rad) - y * std::sin(rad);
        float ry = x * std::sin(rad) + y * std::cos(rad);

        return { round1(rx), round1(ry) };
    }

    // --- Vertex-level operations ---
    inline void translate(Vertex& v, const Attribute& delta)
    {
        if (v.has("position"))
            v.set("position", translate(v.get("position"), delta));
    }

    inline void scale(Vertex& v, float factor)
    {
        if (v.has("position"))
            v.set("position", scale(v.get("position"), factor));
    }

    inline void rotate2D(Vertex& v, float angle)
    {
        if (v.has("position"))
            v.set("position", rotate2D(v.get("position"), angle));
    }

    // --- Triangle-level operations ---
    inline void translate(Triangle& tri, const Attribute& delta)
    {
        for (auto& v : tri.getVertices())
            translate(v, delta);
    }

    inline void scale(Triangle& tri, float factor)
    {
        for (auto& v : tri.getVertices())
            scale(v, factor);
    }

    inline void rotate2D(Triangle& tri, float angle)
    {
        for (auto& v : tri.getVertices())
            rotate2D(v, angle);
    }

    // --- Mesh-level operations ---
    inline void translate(Mesh& mesh, const Attribute& delta)
    {
        for (auto& tri : mesh.getTriangles())
            translate(tri, delta);
    }

    inline void scale(Mesh& mesh, float factor)
    {
        for (auto& tri : mesh.getTriangles())
            scale(tri, factor);
    }

    inline void rotate2D(Mesh& mesh, float angle)
    {
        for (auto& tri : mesh.getTriangles())
            rotate2D(tri, angle);
    }
}

#endif // SCED_MATHOPS_H
