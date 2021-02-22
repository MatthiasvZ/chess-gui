#ifndef POSITION_H
#define POSITION_H


class Position
{
    public:
        Position(unsigned char x, unsigned char y);
        inline unsigned char x() const { return data >> 3; }
        inline unsigned char y() const { return data & 0b0000'0111; }
        inline unsigned char d() const { return data; }

        inline bool operator==(Position b) { return this->data == b.d(); }

    protected:

    private:
        unsigned char data;
};

#endif // POSITION_H
