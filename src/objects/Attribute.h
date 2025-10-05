//
// Created by sethg on 10/4/2025.
//

#ifndef SCED_ATTRIBUTE_H
#define SCED_ATTRIBUTE_H

#include <iostream>
#include <string>
#include <vector>

class Attribute
{
public:
    std::vector<float> data;

    Attribute() {}
    Attribute(std::initializer_list<float> vals) : data(vals) {}

    /*
     *
     */
    float& operator[](size_t i)
    {
        return data.at(i);
    }
    const float& operator[](size_t i) const { return data.at(i); }

    /*
     * returns the size of attributes
     * so if one was of colors,
     */
    size_t size() const
    {
        return data.size();
    }

    void print(const std::string& name = "") const {
        std::cout << name << ": [";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

#endif //SCED_ATTRIBUTE_H