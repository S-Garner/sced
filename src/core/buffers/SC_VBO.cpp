//
// Created by sethg on 10/5/2025.
//

#include "SC_VBO.h"

SC_VBO::SC_VBO()
{

}

SC_VBO::SC_VBO(const int numOfVbOs, const std::vector<float>& vertices)
{
    VBO_size = numOfVbOs;
    glGenBuffers(VBO_size, &VBO_obj);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_obj);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

SC_VBO::~SC_VBO()
{

}
