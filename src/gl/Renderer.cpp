#include "gl/Renderer.h"

Renderer::Renderer()
{

}

void Renderer::clear() const
{
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.12f, 0.16f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawVA(const VertexArray& vao, const IndexBuffer& ibo) const
{
    vao.bindArray();
    ibo.bindBuffer();
    glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
    #ifdef DEBUG
        vao.unbindArray();
        ibo.unbindBuffer();
    #endif // DEBUG
}



Renderer::~Renderer()
{

}
