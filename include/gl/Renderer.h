#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include "gl/VertexArray.h"
#include "gl/IndexBuffer.h"
#include "gl/Shader.h"


class Renderer
{
    public:
        Renderer();
        void clear() const;
        void drawVA(const VertexArray& vao, const IndexBuffer& ibo) const;
        ~Renderer();

    protected:

    private:
};

#endif // RENDERER_H
