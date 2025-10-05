//
// Created by sethg on 10/4/2025.
//

#ifndef SCED_VERTEX_H
#define SCED_VERTEX_H

#include <unordered_map>
#include <string>

#include "Attribute.h"


class Vertex
{
private:
    std::unordered_map<std::string, Attribute> attributes;

public:
    void set(const std::string& key, const Attribute& attr)
    {
        attributes[key] = attr;
    }

    Attribute& get(const std::string& key)
    {
        return attributes.at(key);
    }

    const Attribute& get(const std::string& key) const
    {
        return attributes.at(key);
    }

    bool has(const std::string& key) const
    {
        return attributes.find(key) != attributes.end();
    }

    void print() const
    {
        for (const auto& [name, attr] : attributes)
            attr.print(name);
    }
};

#endif //SCED_VERTEX_H