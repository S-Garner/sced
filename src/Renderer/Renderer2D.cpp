#include "Renderer2D.hpp"
#include "../Renderer/Transform.hpp"
#include <cstddef>

Renderer2D::Renderer2D() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex2D),
        (void*)offsetof(Vertex2D, pos)
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex2D),
        (void*)offsetof(Vertex2D, color)
    );

    glBindVertexArray(0);
}


Renderer2D::~Renderer2D() {
    if (vao) {
        glDeleteVertexArrays(1, &vao);
    }

    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
}

ShapeHandle Renderer2D::addShape(const Vertex2D* verts, int count, const glm::mat4& model) {
    // Check if there are vertices
    if (count <= 0) {
        return {};
    }

    // The current size of verts in cpu, mark where the new shape will be inserted
    const int start = static_cast<int>(cpu.size());

    // Insert the verts into the end of the cpu vertex
    cpu.insert(cpu.end(), verts, verts + count);

    // The GPU needs to be updated
    dirty = true;

    // Create the return record
    ShapeRecord record{};

    // Give it the next unique id
    record.id = nextID++;
    // Where the shape lives on the GPU
    record.offset = start;
    // 
    record.count = count;
    record.model = model;
    shapes.push_back(record);

    return ShapeHandle{record.id};
}

void Renderer2D::setModel(ShapeHandle handle, const glm::mat4& matrix) {
    if (auto* r = find(handle)) r->model = matrix;
}

void Renderer2D::setOverrideColor(ShapeHandle handle, const glm::vec3& color) {
    if (auto* r = find(handle)) { r->useOverride = true; r->overrideColor = color; }
}

void Renderer2D::clearOverrideColor(ShapeHandle handle) {
    if (auto* r = find(handle)) r->useOverride = false;
}

void Renderer2D::updateVertices(ShapeHandle handle, const Vertex2D* verts, int count) {
    auto* record = find(handle);

    if (!record || count != record->count) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        cpu[record->offset + i] = verts[i];
        dirty = true;
    }
}

void Renderer2D::drawAll(const Shader& shader, const glm::mat4& viewProj) {
    if (dirty) {
        upload();
    }

    shader.useShader();
    shader.setViewProj(viewProj);

    
    glBindVertexArray(vao);
    for (const auto& r : shapes) {
        shader.setModel(r.model);
        shader.setUseOverride(r.useOverride);
        if (r.useOverride) shader.setOverride(r.overrideColor);
        glDrawArrays(GL_TRIANGLES, r.offset, r.count);
    }

    glBindVertexArray(0);
}

void Renderer2D::upload() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(cpu.size() * sizeof(Vertex2D)),
                 cpu.data(),
                 GL_DYNAMIC_DRAW);
    dirty = false;
}

ShapeRecord* Renderer2D::find(ShapeHandle h) {
    for (auto& r : shapes) {
        if (r.id == h.id) {
            return &r;
        }
    }
    return nullptr;
}

void Renderer2D::drawShape(const Shader& shader,
                           const glm::mat4& viewProjection,
                           const std::vector<ShapeHandle>& selection)
{
    upload();

    shader.useShader();
    shader.setViewProj(viewProjection);

    glBindVertexArray(vao);

    for (auto& handle : selection)
    {
        auto* r = find(handle);

        if (!r) {
            continue;
        }

        shader.setModel(r->model);
        shader.setUseOverride(r->useOverride);

        if (r->useOverride) {
            shader.setOverride(r->overrideColor);
        }

        glDrawArrays(GL_TRIANGLES, r->offset, r->count);
    }

    glBindVertexArray(0);
}

void Renderer2D::setPosition(ShapeHandle handle, glm::vec2 position) {
    glm::mat4 model = Transform::translate(Transform::setIdentity(), position);
    setModel(handle, model);
}

void Renderer2D::removeShape(ShapeHandle handle) {
    auto it = std::find_if(shapes.begin(), shapes.end(),
        [&](const ShapeRecord& r) { return r.id == handle.id; });

    if (it == shapes.end()) return;

    int removeOffset = it->offset;
    int removeCount  = it->count;

    // Erase vertices from CPU buffer
    cpu.erase(cpu.begin() + removeOffset, cpu.begin() + removeOffset + removeCount);

    // Remove record from shape list
    shapes.erase(it);

    // Update offsets of all shapes after the removed one
    for (auto& r : shapes) {
        if (r.offset > removeOffset)
            r.offset -= removeCount;
    }

    dirty = true;
}