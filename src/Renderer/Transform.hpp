#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Transform {
    glm::mat4 setIdentity() {
        return glm::mat4(1.0f);
    }

    glm::mat4 translate(const glm::mat4& m, glm::vec2 d) {
        return glm::translate(m, glm::vec3(d, 0.0f));
    }

    glm::mat4 scale(const glm::mat4& m, glm::vec2 s) {
        return glm::scale(m, glm::vec3(s, 1.0f));
    }

    glm::mat4 rotateZ(const glm::mat4& m, float radians) {
        return glm::rotate(m, radians, glm::vec3(0,0,1));
    }

    glm::mat4 rotateZ_about(const glm::mat4& m, float r, glm::vec2 pivot) {
        glm::mat4 t = glm::translate(m, glm::vec3(pivot, 0));
        t = glm::rotate(t, r, glm::vec3(0,0,1));
        t = glm::translate(t, glm::vec3(-pivot.x, -pivot.y, 0));
        return t;
    }
    
}