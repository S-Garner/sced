//
// Created by sethg on 10/5/2025.
//

#ifndef SCED_BUFFERDATA_H
#define SCED_BUFFERDATA_H
#include <vector>

#include "../../objects/Mesh.h"

class Mesh;

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
		return data;
    }

    std::vector<float> getColors(const Mesh& mesh);
} // BufferData

#endif //SCED_BUFFERDATA_H
