#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H


class VertexBuffer
{
    public:
        void addData(const float* data, unsigned int count);
        void bindBuffer() const;
        void unbindBuffer() const;
        inline unsigned int getSize() const
        {
            return size;
        }
        ~VertexBuffer();

    protected:

    private:
        unsigned int size;
        unsigned int vboID;
};

#endif // VERTEXBUFFER_H
