//
// Created by sethg on 10/5/2025.
//

#ifndef SCED_SC_VBO_H
#define SCED_SC_VBO_H
#include <vector>

#include "glad/glad.h"


class SC_VBO
{
private:
    const GLuint VBO_obj;
    const GLsizei VBO_size;
public:
    SC_VBO();
    SC_VBO(const int numOfVBOs, const std::vector<float>& vertices);
    ~SC_VBO();

    GLuint getVBO();
    void setVBO(const std::vector<float>& vertices);
};


#endif //SCED_SC_VBO_H