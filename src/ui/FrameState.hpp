#include <glm/glm.hpp>

struct FrameState {
    glm::vec2 worldPos;
    bool mouseDown;
    bool mouseJustPressed;
    bool overButton;
};