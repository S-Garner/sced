#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
private:
    GLuint programID{0};
    GLint modelMatrix{-1}, viewProjMatrix{-1}, useOverride{-1}, uOverride{-1};

    static std::string readFile(const std::string& path);
    static GLuint compile(GLenum type, const char* src);
    static GLuint link(GLuint vs, GLuint fs);
    void cacheUniforms();

public:
    Shader(GLuint program = 0);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    ~Shader();

    static Shader fromFiles(const std::string& vsPath, const std::string& fsPath);

    //Bind
    void useShader() const;

    //Set Uniforms
    void setModel(const glm::mat4& matrix) const;
    void setViewProj(const glm::mat4& matrix) const;
    void setUseOverride(bool boolean) const;
    void setOverride(const glm::vec3& color) const;

    GLuint id() const {
        return programID;
    }
};