#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../Renderer/Renderer2D.hpp"
#include "../Renderer/Transform.hpp"
#include "../Renderer/Shapes/IShape2D.hpp"
#include <unordered_map>

class SCObject {
private:
    Renderer2D* renderer;
    //std::vector<ShapeHandle> shapes;
    glm::mat4 model;
    std::unordered_map<int, ShapeHandle> shapes;
    std::unordered_map<int, glm::mat4> shapeModels;
    bool visible = true;

public:
    SCObject(Renderer2D* renderer);

    ShapeHandle addShape(const std::vector<Vertex2D>& vertices);
    ShapeHandle addShape(const IShape2D& shape);

    void setPosition(const glm::vec2 position);
    void setRotation(float radians);
    void setScale(const glm::vec2 scale);
    void setVisible(bool visible);
    bool isVisible() const;

    void draw(const Shader& shader, const glm::mat4& vp);

    void setShapeColor(ShapeHandle handle, const glm::vec3& color);
    void setShapeModel(ShapeHandle handle, const glm::mat4& model);

    int findShapeIndex(ShapeHandle& handle) const;

    void removeShape(ShapeHandle handle);
};