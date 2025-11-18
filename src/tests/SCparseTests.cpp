#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "../parser/SCparse.hpp"

#ifndef SCPARSE_TEST_JSON_PATH
#error "SCPARSE_TEST_JSON_PATH must be defined"
#endif

int main()
{
    try {
        SCParse::setFile(SCPARSE_TEST_JSON_PATH);
    } catch (const std::exception& ex) {
        std::cerr << "Failed to load test JSON: " << ex.what() << '\n';
        return 1;
    }

    auto rectangle = SCParse::getShape("object_vec");
    auto circle    = SCParse::getShape("array_vec");

    if (!rectangle || !circle) {
        std::cerr << "SCParse returned null shapes" << '\n';
        return 1;
    }

    bool missingShapeThrew = false;
    try {
        (void)SCParse::getShape("does_not_exist");
    } catch (const std::runtime_error&) {
        missingShapeThrew = true;
    }

    if (!missingShapeThrew) {
        std::cerr << "SCParse did not throw for missing shape" << '\n';
        return 1;
    }

    std::cout << "SCParse JSON tests passed" << '\n';
    return 0;
}
