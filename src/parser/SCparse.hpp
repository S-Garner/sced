#pragma once

#include <memory>
#include <string>

#include "../Renderer/Shapes/IShape2D.hpp"   // renderer interface

class SCParse {
public:
    // Load and parse the JSON file once at startup.
    static void setFile(const std::string& path);

    // Build a new shape object from the JSON definition with this name.
    // Usage: auto shape = SCParse::getShape("myFirstCircleShape");
    static std::unique_ptr<IShape2D> getShape(const std::string& name);

private:
    // internal helper: creates the appropriate shape from a JSON node
    static std::unique_ptr<IShape2D> createShape(const std::string& name);
};
