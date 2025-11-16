#include "../Renderer/Shapes/RectangleShape.hpp"

namespace SCArch
{
    inline RectangleShape Rect(
        float width,
        float height,
        const glm::vec2& origin,
        const glm::vec3& color
    )
    {
        float halfWidth = width * 0.5f;
        float halfHeight = height * 0.5f;

        glm::vec2 min(origin.x - halfWidth, origin.y - halfHeight);
        glm::vec2 max(origin.x + halfWidth, origin.y + halfHeight);

        return RectangleShape(min, max, color);
    }
}