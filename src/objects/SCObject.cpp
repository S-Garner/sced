#include "SCObject.hpp"

SCObject::SCObject(Renderer2D* r)
    : renderer(r), model(Transform::setIdentity()) {}

// --- Adds a new shape to the renderer and tracks its handle
ShapeHandle SCObject::addShape(const std::vector<Vertex2D>& vertices) {
    ShapeHandle handle = renderer->addShape(vertices.data(), (int)vertices.size(), model);
    //shapes.push_back(handle);
    shapes[handle.id] = handle;
    shapeModels[handle.id] = Transform::setIdentity();
    return handle;
}

void SCObject::setPosition(const glm::vec2 position) {
    model = Transform::translate(Transform::setIdentity(), position);
    for (auto& [id, handle] : shapes)
        renderer->setModel(handle, model * shapeModels[id]);
}

void SCObject::setRotation(float radians) {
    model = Transform::rotateZ(Transform::setIdentity(), radians);
    for (auto& [id, handle] : shapes)
        renderer->setModel(handle, model * shapeModels[id]);
}

void SCObject::setScale(const glm::vec2 scale) {
    model = Transform::scale(model, scale);
    for (auto& [id, handle] : shapes)
        renderer->setModel(handle, model * shapeModels[id]);
}

// --- Visibility control
void SCObject::setVisible(bool v) {
    visible = v;
}

bool SCObject::isVisible() const {
    return visible;
}

// --- Draw all shapes belonging to this object
void SCObject::draw(const Shader& shader, const glm::mat4& vp) {
    if (!visible) return;

    std::vector<ShapeHandle> shapeHandles;
    shapeHandles.reserve(shapes.size());
    for (const auto& [id, handle] : shapes) {
        shapeHandles.push_back(handle);
    }
    renderer->drawShape(shader, vp, shapeHandles);
}

void SCObject::setShapeColor(ShapeHandle handle, const glm::vec3& color) {
    auto it = shapes.find(handle.id);
    if (it != shapes.end())
        renderer->setOverrideColor(it->second, color);
}

void SCObject::setShapeModel(ShapeHandle handle, const glm::mat4& local) {
    if (auto it = shapes.find(handle.id); it != shapes.end()) {
        shapeModels[handle.id] = local;
        renderer->setModel(handle, model * local); // combine global + local
    }
}

ShapeHandle SCObject::addShape(const IShape2D& shape) {
    return addShape(shape.generateVertices());
}

void SCObject::removeShape(ShapeHandle handle) {
    auto it = shapes.find(handle.id);
    if (it != shapes.end()) {
        renderer->removeShape(handle);
        shapes.erase(it);
    }
}