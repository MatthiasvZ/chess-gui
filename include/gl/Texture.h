#ifndef TEXTURE_H
#define TEXTURE_H

#include "Renderer.h"


class Texture
{
    public:
        Texture(const std::string& path, const unsigned int& slot);
        ~Texture();

        void bindTexture(unsigned int slot = 0) const;
        void unbindTexture() const;

        inline int getWidth() const {return m_Width;}
        inline int getHeight() const {return m_Height;}
        inline int getBPP() const {return m_BPP;}

    protected:

    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP;
};

#endif // TEXTURE_H
