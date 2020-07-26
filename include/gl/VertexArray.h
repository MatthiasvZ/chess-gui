#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
    public:
        void genArray();
        void addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
        void bindArray() const;
        void unbindArray() const;
        ~VertexArray();

    protected:

    private:
        unsigned int vaoID;
};

#endif // VERTEXARRAY_H
