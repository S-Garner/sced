#pragma once
#include <vector>
#include <cstdint>
#include "Vertex2D.hpp"
#include "ShapeRecord.hpp"
#include "Shader/Shader.hpp"

struct ShapeHandle {
    uint32_t id = UINT32_MAX;
};

class Renderer2D {
public:
    Renderer2D();
    ~Renderer2D();

    ShapeHandle addShape(const Vertex2D* verts, int count, const glm::mat4& model = glm::mat4(1.0f));
    void setModel(ShapeHandle handle, const glm::mat4& matrix);
    void setOverrideColor(ShapeHandle handle, const glm::vec3& color);
    void clearOverrideColor(ShapeHandle h);
    void updateVertices(ShapeHandle handle, const Vertex2D* verts, int count);

    void drawAll(const Shader& shader, const glm::mat4& viewProjection);
    void drawShape(const Shader& shader, const glm::mat4& viewProjection, const std::vector<ShapeHandle>& selection);
private:
    GLuint vao{0}, vbo{0};
    std::vector<Vertex2D> cpu;
    std::vector<ShapeRecord> shapes;
    bool dirty{false};
    uint32_t nextID{1};

    void upload();
    ShapeRecord* find(ShapeHandle handle);
};