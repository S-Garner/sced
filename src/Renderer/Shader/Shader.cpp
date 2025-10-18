#include "Shader.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>

Shader::Shader(GLuint program) {
    programID = program;

    if (program) {
        cacheUniforms();
    }
}

Shader Shader::fromFiles(const std::string& vsPath, const std::string& fsPath) {
    const std::string vsRC = readFile(vsPath);
    const std::string fsRC = readFile(fsPath);

    GLuint vs = compile(GL_VERTEX_SHADER, vsRC.c_str());
    GLuint fs = compile(GL_FRAGMENT_SHADER, fsRC.c_str());

    GLuint program = link(vs, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return Shader(program);
}

void Shader::useShader() const {
    glUseProgram(programID);
}

void Shader::setModel(const glm::mat4& matrix) const {
    glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setViewProj(const glm::mat4& matrix) const {
    glUniformMatrix4fv(viewProjMatrix, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUseOverride(bool boolean) const {
    glUniform1i(useOverride, boolean ? 1 : 0);
}

void Shader::setOverride(const glm::vec3& color) const {
    glUniform3fv(uOverride, 1, &color[0]);
}

Shader::Shader(Shader&& other) noexcept {
    programID = other.programID;
    modelMatrix = other.modelMatrix;
    viewProjMatrix = other.viewProjMatrix;
    useOverride = other.useOverride;
    uOverride = other.uOverride;

    other.programID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (programID) {
        glDeleteProgram(programID);
    }

    programID = other.programID;
    modelMatrix = other.modelMatrix;
    viewProjMatrix = other.viewProjMatrix;
    useOverride = other.useOverride;
    uOverride = other.uOverride;

    other.programID = 0;

    return *this;
}

Shader::~Shader() {
    if (programID) {
        glDeleteProgram(programID);
    }
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);

    if (!ifs) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::ostringstream oss;

    oss << ifs.rdbuf();

    return oss.str();
}

GLuint Shader::compile(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);

    glShaderSource(s, 1, &src, nullptr);

    glCompileShader(s);

    GLint check = GL_FALSE;

    glGetShaderiv(s, GL_COMPILE_STATUS, &check);

    if (!check) {
        GLint len = 0;

        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);

        std::string log(len ? len : 1, '\0');

        if (len) {
            glGetShaderInfoLog(s, len, nullptr, log.data());
        }

        glDeleteShader(s);

        const char* t = type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        throw std::runtime_error(std::string(t) + " shader compile failed:\n" + log);
    }

    return s;
}

GLuint Shader::link(GLuint vs, GLuint fs) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    GLint check = GL_FALSE;

    glGetProgramiv(program, GL_LINK_STATUS, &check);

    if (!check) {
        GLint len = 0;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        std::string log(len ? len : 1, '\0');

        if (len) glGetProgramInfoLog(program, len, nullptr, log.data());
        
        glDeleteProgram(program);
        
        throw std::runtime_error("Program link failed:\n" + log);
    }

    return program;
}

void Shader::cacheUniforms() {
    modelMatrix = glGetUniformLocation(programID, "modelMatrix");
    viewProjMatrix = glGetUniformLocation(programID, "viewProjMatrix");
    useOverride = glGetUniformLocation(programID, "useOverrideColor");
    uOverride = glGetUniformLocation(programID, "uOverrideColor");
}