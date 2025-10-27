#pragma once

class Window;
class Input;
class Renderer2D;
class Shader;

#include <memory>
#include <string>

class Application {
public:
    Application(int width, int height, const char* title);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();

private:
    void initialize();
    void mainLoop();
    void cleanup();

    int m_Width;
    int m_Height;
    std::string m_Title;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Input> m_Input;
    std::unique_ptr<Renderer2D> m_Renderer;
    std::unique_ptr<Shader> m_Shader;
};