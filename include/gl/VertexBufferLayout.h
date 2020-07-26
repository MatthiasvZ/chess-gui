
#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include "GL/glew.h"


struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalised;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:          return 4;
            case GL_INT:            return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        return 0;
    }
};

class VertexBufferLayout
{
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;

    public:
        VertexBufferLayout()
            : m_Elements(), m_Stride(0) {}

        void push(unsigned int attribType, unsigned int count)
        {
            m_Elements.push_back({attribType, count, GL_FALSE});
            m_Stride += count * VertexBufferElement::getSizeOfType(attribType);
        }

        inline const std::vector<VertexBufferElement>& getElements() const {return m_Elements;}
        inline unsigned int getStride() const {return m_Stride;}
    protected:
};

#endif // VERTEXBUFFERLAYOUT_H
