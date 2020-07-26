#include "gl/VertexArray.h"

#include <GL/glew.h>

void VertexArray::genArray()
{
    glGenVertexArrays(1, &vaoID);
}

void VertexArray::addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
    bindArray();
    vbo.bindBuffer();
    const auto& elements = layout.getElements();
    unsigned long offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, \
                element.normalised, layout.getStride(), (const void*)offset);
        offset += element.count*VertexBufferElement::getSizeOfType(element.type);
    }
    #ifdef DEBUG
        unbindArray();
    #endif // DEBUG
}

void VertexArray::bindArray() const
{
    glBindVertexArray(vaoID);
}

void VertexArray::unbindArray() const
{
    glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vaoID);
}
