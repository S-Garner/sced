#include "SCparse.hpp"

#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/common.hpp>

#include "../Renderer/Shapes/CircleShape.hpp"
#include "../Renderer/Shapes/RectangleShape.hpp"
#include "../Renderer/Shapes/EllipseShape.hpp"
#include "../Renderer/Shapes/RegularPolygonShape.hpp"
#include "../Renderer/Shapes/IShape2D.hpp"
#include "../color/SColor.hpp"
#include "../converter/SCArch.cpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ---------- static storage ----------
static json g_doc;
static bool g_loaded = false;
static std::unordered_map<std::string, json> g_shapeDefs;

// ---------- helpers ----------

static glm::vec2 readVec2(const json& j, const char* key,
                          glm::vec2 def = glm::vec2(0.0f))
{
    if (!j.contains(key)) return def;
    const auto& obj = j[key];

    if (obj.is_object()) {
        return glm::vec2(
            obj.value("x", def.x),
            obj.value("y", def.y)
        );
    }

    if (obj.is_array() && obj.size() >= 2 && obj[0].is_number() && obj[1].is_number()) {
        return glm::vec2(
            obj[0].get<float>(),
            obj[1].get<float>()
        );
    }

    return def;
}

static glm::vec3 readColorRgb(const json& j, const char* key)
{
    if (!j.contains(key)) return glm::vec3(1.0f);

    const auto& arr = j[key];
    if (!arr.is_array() || arr.size() < 3) return glm::vec3(1.0f);

    auto normalizeComponent = [](const json& value) -> float {
        if (!value.is_number()) {
            return 1.0f;
        }

        if (value.is_number_integer() || value.get<float>() > 1.0f) {
            const int componentInt = value.get<int>();
            return glm::clamp(SColor::normalizeColorComponent(componentInt), 0.0f, 1.0f);
        }

        return glm::clamp(value.get<float>(), 0.0f, 1.0f);
    };

    return glm::vec3(
        normalizeComponent(arr[0]),
        normalizeComponent(arr[1]),
        normalizeComponent(arr[2])
    );
}

// ---------- SCParse API ----------

void SCParse::setFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("SCParse::setFile - cannot open " + path);
    }

    file >> g_doc;
    g_loaded = true;
    g_shapeDefs.clear();

    if (g_doc.contains("shapes") && g_doc["shapes"].is_object()) {
        for (auto it = g_doc["shapes"].begin(); it != g_doc["shapes"].end(); ++it) {
            g_shapeDefs[it.key()] = it.value();
        }
    }
}

std::unique_ptr<IShape2D> SCParse::getShape(const std::string& name)
{
    if (!g_loaded) {
        throw std::runtime_error("SCParse::getShape called before setFile()");
    }

    return createShape(name);
}

std::unique_ptr<IShape2D> SCParse::createShape(const std::string& name)
{
    auto it = g_shapeDefs.find(name);
    if (it == g_shapeDefs.end()) {
        throw std::runtime_error("SCParse::getShape - shape \"" + name + "\" not found");
    }

    const json& node = it->second;

    std::string type = node.value("type", std::string{});

    glm::vec2 pos   = readVec2(node, "position");
    glm::vec3 color = readColorRgb(node, "color");

    // ---- circle ----
    if (type == "circle") {
        float radius   = node.value("radius",   0.1f);
        int   segments = node.value("segments", 32);
        return std::make_unique<CircleShape>(pos, radius, segments, color);
    }

    // ---- rectangle ----
    if (type == "rectangle") {
        glm::vec2 size = readVec2(node, "size", glm::vec2(0.5f, 0.25f));
        RectangleShape rect = SCArch::Rect(size.x, size.y, pos, color);
        return std::make_unique<RectangleShape>(rect);
    }

    // ---- ellipse ----
    if (type == "ellipse") {
        glm::vec2 radii = readVec2(node, "radii", glm::vec2(0.5f, 0.25f));
        int segments    = node.value("segments", 64);
        // EllipseShape(glm::vec2 center, glm::vec2 radii, int segments, glm::vec3 color)
        return std::make_unique<EllipseShape>(pos, radii, segments, color);
    }

    // ---- regular polygon ----
    if (type == "regular_polygon") {
        float radius = node.value("radius", 0.5f);
        int   sides  = node.value("sides",  6);
        // RegularPolygonShape(glm::vec2 center, float radius, int sides, glm::vec3 color)
        return std::make_unique<RegularPolygonShape>(pos, radius, sides, color);
    }

    throw std::runtime_error("SCParse::getShape - unsupported shape type \"" + type + "\"");
}
