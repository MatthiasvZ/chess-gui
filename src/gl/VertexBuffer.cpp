#include "gl/VertexBuffer.h"

#include "GL/glew.h"

void VertexBuffer::addData(const float* data, unsigned int size)
{
    this->size = size;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    #ifdef DEBUG
        unbindBuffer();
    #endif // DEBUG
}

void VertexBuffer::bindBuffer() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

void VertexBuffer::unbindBuffer() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &vboID);
}
