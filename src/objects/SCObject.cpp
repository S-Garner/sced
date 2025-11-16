#include "SCObject.hpp"

SCObject::SCObject(Renderer2D* r)
    : renderer(r)
{
    model = glm::mat4(1.f);
}

// -------------------------------
// Adding Shapes
// -------------------------------
ShapeHandle SCObject::addShape(const std::vector<Vertex2D>& vertices) {
    ShapeHandle handle = renderer->addShape(vertices.data(), (int)vertices.size(), model);
    shapes[handle.id] = handle;
    localModels[handle.id] = glm::mat4(1.f);
    return handle;
}

ShapeHandle SCObject::addShape(const IShape2D& shape) {
    return addShape(shape.generateVertices());
}

// -------------------------------
// Setting Local Shape Transform
// -------------------------------
void SCObject::setShapeModel(ShapeHandle handle, const glm::mat4& local) {
    if (shapes.find(handle.id) == shapes.end()) return;
    localModels[handle.id] = local;
    renderer->setModel(handle, model * local);
}

// -------------------------------
// Global Transform System
// -------------------------------
glm::mat4 SCObject::buildModel() const {
    glm::mat4 m(1.f);
    m = Transform::translate(m, globalPos);
    m = Transform::rotateZ(m, globalRot);
    m = Transform::scale(m, globalScale);
    return m;
}

void SCObject::updateAllModels() {
    model = buildModel();
    for (auto& [id, handle] : shapes)
        renderer->setModel(handle, model * localModels[id]);
}

void SCObject::setPosition(const glm::vec2 position) {
    globalPos = position;
    updateAllModels();
}

void SCObject::setRotation(float radians) {
    globalRot = radians;
    updateAllModels();
}

void SCObject::setScale(const glm::vec2 scale) {
    globalScale = scale;
    updateAllModels();
}

// -------------------------------
// Visibility
// -------------------------------
void SCObject::setVisible(bool v) {
    visible = v;
}

bool SCObject::isVisible() const {
    return visible;
}

// -------------------------------
// Color
// -------------------------------
void SCObject::setShapeColor(ShapeHandle handle, const glm::vec3& color) {
    auto it = shapes.find(handle.id);
    if (it != shapes.end())
        renderer->setOverrideColor(handle, color);
}

// -------------------------------
// Draw
// -------------------------------
void SCObject::draw(const Shader& shader, const glm::mat4& vp) {
    if (!visible) return;

    std::vector<ShapeHandle> list;
    list.reserve(shapes.size());

    for (auto& [id, handle] : shapes)
        list.push_back(handle);

    renderer->drawShape(shader, vp, list);
}

// -------------------------------
// Clone
// -------------------------------
SCObject SCObject::clone() const {
    SCObject copy(renderer);
    copy.visible = visible;

    copy.globalPos = globalPos;
    copy.globalRot = globalRot;
    copy.globalScale = globalScale;

    copy.model = model;

    // clone each shape
    for (auto& [id, handle] : shapes)
    {
        const ShapeRecord* record = renderer->getRecord(handle);
        if (!record) continue;

        const auto& cpuBuf = renderer->getCPUBuffer();

        std::vector<Vertex2D> verts(
            cpuBuf.begin() + record->offset,
            cpuBuf.begin() + record->offset + record->count
        );

        ShapeHandle newHandle = renderer->addShape(verts.data(), (int)verts.size(), model);
        copy.shapes[newHandle.id] = newHandle;

        // copy per-shape local model
        auto it = localModels.find(id);
        if (it != localModels.end())
            copy.localModels[newHandle.id] = it->second;
        else
            copy.localModels[newHandle.id] = glm::mat4(1.f);
    }

    return copy;
}
