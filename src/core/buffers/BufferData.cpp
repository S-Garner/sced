//
// Created by sethg on 10/5/2025.
//

#include "BufferData.h"

namespace BufferData
{
    std::vector<float> getData(const Mesh& mesh)
    {
        std::vector<float> data;

        for (const auto& tri : mesh.getTriangles())
        {
            for (const auto& v : tri.getVertices())
            {
                if (v.has("position"))
                {
                        const auto& pos = v.get("position").data;
                        data.insert(data.end(), pos.begin(), pos.end());
                }

                if (v.has("color"))
                {
                        const auto& col = v.get("color").data;
                        data.insert(data.end(), col.begin(), col.end());
                }
            }
        }
        return data;
    }

    std::vector<float> getPositions(const Mesh& mesh)
    {
        std::vector<float> data;

        for (const auto& tri : mesh.getTriangles())
        {
            for (const auto& v : tri.getVertices())
            {
                if (v.has("position"))
                {
                    const auto& pos = v.get("position").data;
                }
            }
        }
    }

    std::vector<float> getColors(const Mesh& mesh)
    {
        std::vector<float> data;

        for (const auto& tri : mesh.getTriangles())
        {
            for (const auto& v : tri.getVertices())
            {
                if (v.has("color"))
                {
                    const auto& col = v.get("color").data;
                }
            }
        }
    }

} // BufferData