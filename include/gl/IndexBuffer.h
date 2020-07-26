#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H


class IndexBuffer
{
    public:
        void addData(const unsigned int* data, unsigned int count);
        void bindBuffer() const;
        void unbindBuffer() const;
        inline unsigned int getCount() const
        {
            return count;
        }
        ~IndexBuffer();

    protected:

    private:
        unsigned int count;
        unsigned int iboID;
};

#endif // INDEXBUFFER_H
