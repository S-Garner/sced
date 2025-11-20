#pragma once
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

#include "../Renderer/Renderer2D.hpp"
#include "../Renderer/Transform.hpp"
#include "../Renderer/Shapes/IShape2D.hpp"

class SCObject {
private:
    Renderer2D* renderer;

    // RAW TRANSFORM COMPONENTS (no accumulation)
    glm::vec2  globalPos    = {0.f, 0.f};
    float      globalRot    = 0.f;
    glm::vec2  globalScale  = {1.f, 1.f};

    glm::mat4 model = glm::mat4(1.f);

    std::unordered_map<int, ShapeHandle> shapes;
    std::unordered_map<int, glm::mat4> localModels; // per-shape offsets

    bool visible = true;

    // rebuild model and push to renderer
    void updateAllModels();

    glm::mat4 buildModel() const;

public:
    SCObject(Renderer2D* renderer);

    ShapeHandle addShape(const std::vector<Vertex2D>& vertices);
    ShapeHandle addShape(const IShape2D& shape);

    void setShapeModel(ShapeHandle handle, const glm::mat4& local);
    void setShapeColor(ShapeHandle handle, const glm::vec3& color);

    // global transforms
    void setPosition(const glm::vec2 position);
    void setRotation(float radians);
    void setScale(const glm::vec2 scale);

    void setVisible(bool v);
    bool isVisible() const;

    void draw(const Shader& shader, const glm::mat4& vp) const;

    SCObject clone() const;

    std::vector<ShapeHandle> getShapeHandles() const {
        std::vector<ShapeHandle> out;
        out.reserve(shapes.size());
        for (auto& [id, h] : shapes)
            out.push_back(h);
        return out;
    }
};
