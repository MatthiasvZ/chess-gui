#ifndef POSITION_H
#define POSITION_H


class Position
{
    public:
        Position(unsigned char x, unsigned char y);
        inline unsigned char x() { return data % 8; }
        inline unsigned char y() { return data >> 4; }
        inline unsigned char d() { return 8*x() + y(); }
        inline unsigned char b() { return data; }

        inline bool operator==(Position b) { return this->data == b.b(); }

    protected:

    private:
        unsigned char data;
};

#endif // POSITION_H
