#include "Application/Application.h"
#include <iostream>

int main() {
    std::cout << "Create app" << std::endl;
    Application app(800, 600, "SCEd 2D Uniform Renderer");

    std::cout << "Running app" << std::endl;
    app.run();

    std::cout << "App closed" << std::endl;
    std::cout << "Goodbye!" << std::endl;
    return 0;
}
