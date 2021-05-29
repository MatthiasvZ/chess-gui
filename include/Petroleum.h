#ifndef PETROLEUM_H
#define PETROLEUM_H

#include <vector>
#include <unordered_map>
#include <string>

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define PT_SHADER_XY 0
#define PT_SHADER_XYUV 1
#define PT_SHADER_XYA 2
#define PT_SHADER_XYAUV 3
#define PT_SHADER_XYB 4
#define PT_SHADER_XYBUV 5
#define PT_SHADER_XYRGB 6
#define PT_SHADER_XYRGBUV 7
#define PT_SHADER_XYRGBA 8
#define PT_SHADER_XYRGBAUV 9
#define PT_SHADER_XY_M 10
#define PT_SHADER_XYUV_M 11
#define PT_SHADER_XYA_M 12
#define PT_SHADER_XYAUV_M 13
#define PT_SHADER_XYB_M 14
#define PT_SHADER_XYBUV_M 15
#define PT_SHADER_XYRGB_M 16
#define PT_SHADER_XYRGBUV_M 17
#define PT_SHADER_XYRGBA_M 18
#define PT_SHADER_XYRGBAUV_M 19
#define PT_SHADER_XYZ_M 20
#define PT_SHADER_XYZUV_M 21
#define PT_SHADER_XYZA_M 22
#define PT_SHADER_XYZAUV_M 23
#define PT_SHADER_XYZB_M 24
#define PT_SHADER_XYZBUV_M 25
#define PT_SHADER_XYZRGB_M 26
#define PT_SHADER_XYZRGBUV_M 27
#define PT_SHADER_XYZRGBA_M 28
#define PT_SHADER_XYZRGBAUV_M 29

#define PT_BLACK 0
#define PT_WHITE 1
#define PT_SKY_BLUE 2
#define PT_PETROL 3
#define PT_LIGHT_GREY 4
#define PT_GREY 5
#define PT_DARK_GREY 6

#define PT_VERSION_S "0.4.93-dev"
#define PT_VERSION   4'93 // 1.2.5 = 1'02'05

namespace PT
{

// FILE MANAGEMENT
void setDataDir(const char* directory);
void createDataFolder(const char* directory);
void createFolder(const char* directory);
struct Config
{
    bool fresh {true};

    unsigned int version {PT_VERSION};
    unsigned int opengl_major {4};
    unsigned int opengl_minor {0};
    bool vsync {false};
    unsigned int msaa {4};
    bool fullscreen {false};
    unsigned int clear_colour {PT_BLACK};
    bool enable_blending {true};
    bool capture_mouse {false};
    bool window_resizable {false};
    bool print_status {false};
};
Config parseConfig();
void saveConfig(Config cfg);


// CORE
void initGL();
void initGL(Config cfg);
void doEvents();
void printVersion();


// ERROR HANDLING

struct DebugInfo
{
    std::string call;
    int line = 0;
    std::string func;
    std::string file;
};

void clearGlErrors();
void getGlErrors(DebugInfo* dbi = nullptr);
void clearAlErrors();
void getAlErrors(DebugInfo* dbi = nullptr);

#ifdef DEBUG
#define GLEC(FUNCTION_CALL)\
FUNCTION_CALL;\
{\
DebugInfo pt_dbi_;\
pt_dbi_.call = #FUNCTION_CALL;\
pt_dbi_.line = __LINE__;\
pt_dbi_.func = __func__;\
pt_dbi_.file = __FILE__;\
PT::getGlErrors(&pt_dbi_);\
}

#define PTGLEC(FUNCTION_CALL)\
FUNCTION_CALL;\
{\
DebugInfo pt_dbi_;\
pt_dbi_.call = #FUNCTION_CALL;\
pt_dbi_.func = __func__;\
PT::getGlErrors(&pt_dbi_);\
}

#define ALEC(FUNCTION_CALL)\
FUNCTION_CALL;\
{\
DebugInfo pt_dbi_;\
pt_dbi_.call = #FUNCTION_CALL;\
pt_dbi_.line = __LINE__;\
pt_dbi_.func = __func__;\
pt_dbi_.file = __FILE__;\
PT::getAlErrors(&pt_dbi_);\
}

#define PTALEC(FUNCTION_CALL)\
FUNCTION_CALL;\
{\
DebugInfo pt_dbi_;\
pt_dbi_.call = #FUNCTION_CALL;\
pt_dbi_.func = __func__;\
PT::getAlErrors(&pt_dbi_);\
}

#else
#define GLEC(FUNCTION_CALL)\
FUNCTION_CALL;

#define PTGLEC(FUNCTION_CALL)\
FUNCTION_CALL;

#define ALEC(FUNCTION_CALL)\
FUNCTION_CALL;

#define PTALEC(FUNCTION_CALL)\
FUNCTION_CALL;
#endif // DEBUG


class Window
{
    public:
        Window();
        Window(Config cfg);
        bool shouldRun() const;
        bool getKey(int glfwKey) const;
        bool getMouseButton(int glfwButton) const;
        int focused() const;
        bool mouseLocked() const;
        void getCursorPos(double* p_X, double* p_Y) const;
        void update();
        void changeTitle(const std::string newTitle);
        void makeContextCurrent();
        inline GLFWwindow* getGLFWWindow() { return window; }

        virtual ~Window();

    private:
        GLFWwindow* window;
        void init();

        int fps, seconds, avg_fps, last_fps;
        long int tn, ta;
        std::string title;
};


class IndexBuffer
{
    public:
        IndexBuffer(const std::vector<unsigned int>& data, unsigned int drawType = GL_DYNAMIC_DRAW);
        IndexBuffer(const std::vector<unsigned short>& data, unsigned int drawType = GL_DYNAMIC_DRAW);
        IndexBuffer(const std::vector<unsigned char>& data, unsigned int drawType = GL_DYNAMIC_DRAW);
        IndexBuffer(const unsigned int data[], unsigned int size, unsigned int drawType = GL_DYNAMIC_DRAW);
        IndexBuffer(const unsigned short data[], unsigned int size, unsigned int drawType = GL_DYNAMIC_DRAW);
        IndexBuffer(const unsigned char data[], unsigned int size, unsigned int drawType = GL_DYNAMIC_DRAW);
        void bindBuffer() const;
        void unbindBuffer() const;
        void updateData(const std::vector<unsigned int>& data);
        void updateData(const std::vector<unsigned short>& data);
        void updateData(const std::vector<unsigned char>& data);
        void updateData(const unsigned int data[], unsigned int size);
        void updateData(const unsigned short data[], unsigned int size);
        void updateData(const unsigned char data[], unsigned int size);
        inline unsigned int getIboID() const { return iboID; }
        inline unsigned int getCount() const { return count; }
        inline unsigned int getDataType() const { return dataType; }
        inline const void* getDataPointer() const { return data; }
        inline void setDrawType(unsigned int drawType) { this->drawType = drawType; }
        void remove();
        ~IndexBuffer();

    private:
        unsigned int iboID;
        unsigned int dataType;
        long unsigned int count;
        unsigned int drawType;
        const void* data;
};


class VertexBuffer
{
    public:
        VertexBuffer(const std::vector<float>& data, unsigned int drawType = GL_DYNAMIC_DRAW);
        VertexBuffer(const float data[], unsigned int size, unsigned int drawType = GL_DYNAMIC_DRAW);
        void bindBuffer() const;
        void unbindBuffer() const;
        void updateData(const std::vector<float>& data);
        void updateData(const float data[], unsigned int size);
        inline unsigned int getVboID() const { return vboID; }
        inline void setDrawType(unsigned int drawType) { this->drawType = drawType; }
        void remove();
        ~VertexBuffer();

    private:
        unsigned int vboID;
        unsigned int drawType;
};


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
        std::vector<VertexBufferElement> elements;
        unsigned int stride;

    public:
        VertexBufferLayout()
            : elements(), stride(0) {}

        void push(unsigned int attribType, unsigned int count)
        {
            elements.push_back({attribType, count, GL_FALSE});
            stride += count * VertexBufferElement::getSizeOfType(attribType);
        }

        inline const std::vector<VertexBufferElement>& getElements() const {return elements;}
        inline unsigned int getStride() const {return stride;}
    protected:
};


std::string readFromFile(const char* filePath);

struct SourcePackage
{
    std::string vertex;
    std::string fragment;
};

class Shader
{
    public:
        Shader(unsigned int shaderName);
        Shader(SourcePackage srcpkg);
        ~Shader();

        void bindShader() const;
        void unbindShader() const;

        void setUniform1i(const std::string& name, int v0);
        void setUniform2i(const std::string& name, int v0, int v1);
        void setUniform3i(const std::string& name, int v0, int v1, int v2);
        void setUniform4i(const std::string& name, int v0, int v1, int v2, int v3);
        void setUniform1f(const std::string& name, float v0);
        void setUniform2f(const std::string& name, float v0, float v1);
        void setUniform3f(const std::string& name, float v0, float v1, float v2);
        void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void setUniformMat2f(const std::string& name, const glm::mat2& mat);
        void setUniformMat3f(const std::string& name, const glm::mat3& mat);
        void setUniformMat4f(const std::string& name, const glm::mat4& mat);

        inline void setMat(const glm::mat4& mat) { setUniformMat4f("u_Mat", mat); }
        inline void setTexSlot(int texSlot) { setUniform1i("u_TexSlot", texSlot); }

        inline VertexBufferLayout getLayout() { return layout; }

    private:
        unsigned int programID;
        VertexBufferLayout layout;
        VertexBufferLayout createLayout(std::string vertexSource);
        std::unordered_map<std::string, int> uniformLocationCache;
        unsigned int compileShader(const char* src, unsigned int type);
        int getUniformLocation(const std::string& name);
};

class VertexArray
{
    public:
        VertexArray();
        void addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
        void bindArray() const;
        void unbindArray() const;
        void remove();
        inline unsigned int getVBCount() const { return vbCount; }
        ~VertexArray();

    private:
        unsigned int vaoID;
        unsigned int vbCount;
};


class Texture
{
    public:
        Texture(const std::string& path, const unsigned int& slot, unsigned int minFilter = GL_NEAREST_MIPMAP_NEAREST, unsigned int magFilter = GL_NEAREST);
        Texture(unsigned long int bufferLength, const unsigned char* imageBuffer, const unsigned int& slot, unsigned int minFilter = GL_NEAREST_MIPMAP_NEAREST, unsigned int magFilter = GL_NEAREST);
        virtual ~Texture();

        void bindTexture(unsigned int slot = 0) const;
        void unbindTexture() const;

        inline int getWidth() const {return width;}
        inline int getHeight() const {return height;}
        inline int getBPP() const {return bPP;}

    protected:
        unsigned int texID;
        std::string filePath;
        unsigned char* localBuffer;
        int width, height, bPP;
};

class TextureAtlas : public Texture
{
    public:
        TextureAtlas(const std::string& path, const unsigned int& slot, unsigned int minFilter = GL_NEAREST_MIPMAP_NEAREST, unsigned int magFilter = GL_NEAREST);
        TextureAtlas(unsigned long int bufferLength, const unsigned char* imageBuffer, const unsigned int& slot, unsigned int minFilter = GL_NEAREST_MIPMAP_NEAREST, unsigned int magFilter = GL_NEAREST);

        inline void setIndexSize(float n) { indexSize = n; }

        inline float getU1(float offset) { return offset * indexSize / width; }
        inline float getU2(float offset) { return offset * indexSize / width; }
        inline float getU3(float offset) { return (offset + 1) * indexSize / width; }
        inline float getU4(float offset) { return (offset + 1) * indexSize / width; }

        inline float getV1(float offset) { return offset * indexSize / height; }
        inline float getV2(float offset) { return (offset + 1) * indexSize / height; }
        inline float getV3(float offset) { return (offset + 1) * indexSize / height; }
        inline float getV4(float offset) { return offset * indexSize / height; }

    private:
        float indexSize;
};


class Camera
{
    public:
        Camera(float x = 1.0f, float y = 1.0f, float z = 1.0f);
        glm::mat4 update(float deltaTime, const Window& window);
        inline void setClippingDistance(float s) { clippingDistance = s; }
        inline void setSpeedH(float s) { movFacH = s; }
        inline void setSpeedV(float s) { movFacV = s; }
        inline void setMouseSpeed(float s) { turnSpeed = s; }

        inline float getPosX() { return camPos[0]; }
        inline float getPosY() { return camPos[1]; }
        inline float getPosZ() { return camPos[2]; }

        inline void setPosX(float x) { camPos[0] = x; }
        inline void setPosY(float y) { camPos[1] = y; }
        inline void setPosZ(float z) { camPos[2] = z; }

        inline float getPitch() { return pitch; }
        inline float getYaw() { return yaw; }

    private:
        float clippingDistance;
        float movFacH;
        float movFacV;
        float turnSpeed;
        float pitch;
        float yaw;
        bool sprinting;
        glm::vec3 camPos;
        glm::vec3 camFront;
        double lastMouseX;
        double lastMouseY;
        float initX;
        float initY;
        float initZ;
};


// RENDERER
void clearScreen();
void drawVA(const VertexArray& vao, const IndexBuffer& ibo);
void drawVA(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);
void drawMultiVA(const VertexArray& vao, const IndexBuffer* const* ibo);
void drawMultiVA(const VertexArray& vao, const IndexBuffer* const* ibo, const Shader& shader);


// AUDIO

void initAL();

void setAlListenerPos(float x, float y, float z);
void setAlListenerVel(float x, float y, float z);
void setAlListenerOrientation(float* forwardAndUpVectors);

class Audio
{
    public:
        Audio(const char* file);
        ~Audio();

        void play();
        void stop();

        void setPos(float x, float y, float z);
        void setVel(float x, float y, float z);
        void setPitch(float n);
        void setGain(float n);

        bool isPlaying();

    private:
        unsigned int channels;
        unsigned int sampleRate;
        unsigned long long int totalPCMFrameCount;

        std::vector<int16_t> data;
        unsigned int soundBuffer;

        unsigned int soundSource;
};


// Template vertices
std::vector<float> tVertsEquilateralXY(float posX, float posY, float size, bool centred = true);
std::vector<float> tVertsEquilateralXYUV(float posX, float posY, float size, bool centred = true);
std::vector<float> tVertsSquareXY(float posX, float posY, float size, bool centred = false);
std::vector<float> tVertsSquareXYUV(float posX, float posY, float size, bool centred = false);
std::vector<float> tVertsCubeXYZ(float posX, float posY, float posZ, float size, bool centred = false, bool shortened = true);
std::vector<float> tVertsCubeXYZUV(float posX, float posY, float posZ, float size, bool centred = false, bool shortened = true);

std::vector<float> xyToXyz(const std::vector<float>& vertices2d, unsigned int vertexSize, float z = 0.0f);

// Apparently, you can't prototype template funcs, lmao
template <typename T>
inline std::vector<T> tIndsTriangles(T count)
{
    std::vector<T> result(3 * count);
    for (T i {0}; i < count; ++i)
    {
        result.push_back(0 + 3*i);
        result.push_back(1 + 3*i);
        result.push_back(2 + 3*i);
    }
    return result;
}

template <typename T>
inline std::vector<T> tIndsSquares(T count)
{
    std::vector<T> result(6 * count);
    for (T i {0}; i < count; ++i)
    {
        result.push_back(0 + 4*i);
        result.push_back(1 + 4*i);
        result.push_back(2 + 4*i);

        result.push_back(0 + 4*i);
        result.push_back(2 + 4*i);
        result.push_back(3 + 4*i);
    }
    return result;
}

template <typename T>
inline std::vector<T> tIndsCubes(T count, bool shortened = true)
{
    if (shortened)
    {
        std::vector<T> result(36 * count);
        for (T i {0}; i < count; ++i)
        {
            result.push_back(0 + 8*i);
            result.push_back(1 + 8*i);
            result.push_back(2 + 8*i);

            result.push_back(1 + 8*i);
            result.push_back(2 + 8*i);
            result.push_back(3 + 8*i);


            result.push_back(4 + 8*i);
            result.push_back(5 + 8*i);
            result.push_back(6 + 8*i);

            result.push_back(5 + 8*i);
            result.push_back(6 + 8*i);
            result.push_back(7 + 8*i);


            result.push_back(0 + 8*i);
            result.push_back(1 + 8*i);
            result.push_back(4 + 8*i);

            result.push_back(1 + 8*i);
            result.push_back(4 + 8*i);
            result.push_back(5 + 8*i);


            result.push_back(2 + 8*i);
            result.push_back(3 + 8*i);
            result.push_back(6 + 8*i);

            result.push_back(3 + 8*i);
            result.push_back(6 + 8*i);
            result.push_back(7 + 8*i);


            result.push_back(0 + 8*i);
            result.push_back(2 + 8*i);
            result.push_back(4 + 8*i);

            result.push_back(2 + 8*i);
            result.push_back(4 + 8*i);
            result.push_back(6 + 8*i);


            result.push_back(1 + 8*i);
            result.push_back(3 + 8*i);
            result.push_back(5 + 8*i);

            result.push_back(3 + 8*i);
            result.push_back(5 + 8*i);
            result.push_back(7 + 8*i);
        }
        return result;
    }
    else
        return tIndsSquares<T>(6);
}

template <typename T>
inline std::vector<T> tIndsTexturedCubes(T count, bool shortened = true)
{
    if (shortened)
    {
        std::vector<T> result(36 * count);
        for (T i {0}; i < count; ++i)
        {
            result.push_back(0 + 12*i);
            result.push_back(1 + 12*i);
            result.push_back(2 + 12*i);

            result.push_back(1 + 12*i);
            result.push_back(2 + 12*i);
            result.push_back(3 + 12*i);


            result.push_back(4 + 12*i);
            result.push_back(5 + 12*i);
            result.push_back(6 + 12*i);

            result.push_back(5 + 12*i);
            result.push_back(6 + 12*i);
            result.push_back(7 + 12*i);


            result.push_back(8 + 12*i);
            result.push_back(1 + 12*i);
            result.push_back(9 + 12*i);

            result.push_back(1 + 12*i);
            result.push_back(9 + 12*i);
            result.push_back(5 + 12*i);


            result.push_back(10 + 12*i);
            result.push_back(3 + 12*i);
            result.push_back(11 + 12*i);

            result.push_back(3 + 12*i);
            result.push_back(11 + 12*i);
            result.push_back(7 + 12*i);


            result.push_back(0 + 12*i);
            result.push_back(2 + 12*i);
            result.push_back(4 + 12*i);

            result.push_back(2 + 12*i);
            result.push_back(4 + 12*i);
            result.push_back(6 + 12*i);


            result.push_back(1 + 12*i);
            result.push_back(3 + 12*i);
            result.push_back(5 + 12*i);

            result.push_back(3 + 12*i);
            result.push_back(5 + 12*i);
            result.push_back(7 + 12*i);
        }
        return result;
    }
    else
        return tIndsSquares<T>(6);
}

}


#endif // PETROLEUM_H
