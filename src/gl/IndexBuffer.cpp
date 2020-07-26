#include "gl/IndexBuffer.h"

#include "GL/glew.h"

void IndexBuffer::addData(const unsigned int* data, unsigned int count)
{
    this->count = count;
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
    #ifdef DEBUG
        unbindBuffer();
    #endif // DEBUG
}

void IndexBuffer::bindBuffer() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
}

void IndexBuffer::unbindBuffer() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &iboID);
}
