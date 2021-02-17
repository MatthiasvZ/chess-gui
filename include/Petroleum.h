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
#define PT_SHADER_XYZ_M 10
#define PT_SHADER_XYZUV_M 11
#define PT_SHADER_XYZA_M 12
#define PT_SHADER_XYZAUV_M 13
#define PT_SHADER_XYZB_M 14
#define PT_SHADER_XYZBUV_M 15
#define PT_SHADER_XYZRGB_M 16
#define PT_SHADER_XYZRGBUV_M 17
#define PT_SHADER_XYZRGBA_M 18
#define PT_SHADER_XYZRGBAUV_M 19

#define PT_BLACK 0
#define PT_WHITE 1
#define PT_SKY_BLUE 2
#define PT_PETROL 3
#define PT_LIGHT_GREY 4
#define PT_GREY 5
#define PT_DARK_GREY 6

namespace PT
{

// FILE MANAGEMENT
void setDataDir(const char* directory);
void createDataFolder(const char* directory);
void createFolder(const char* directory);
struct Config
{
    bool fresh {true};

    unsigned int opengl_major {4};
    unsigned int opengl_minor {0};
    bool vsync {false};
    unsigned int msaa {4};
    bool fullscreen {false};
    unsigned int clear_colour {PT_BLACK};
    bool enable_blending {true};
    bool capture_mouse {false};
    bool window_resizable {false};
};
Config parseConfig();
void saveConfig(Config cfg);


// CORE
void initGL();
void initGL(Config cfg);
void doEvents();


// ERROR HANDLING
void clearGlErrors();
void getGlErrors();


struct Input
{
        bool ctrlHeld {false};
        bool spaceHeld {false};
        bool leftShiftHeld {false};
        bool rightShiftHeld {false};
        bool tabHeld {false};
        bool qHeld {false};
        bool wHeld {false};
        bool eHeld {false};
        bool rHeld {false};
        bool aHeld {false};
        bool sHeld {false};
        bool dHeld {false};
        bool fHeld {false};
        bool yHeld {false};
        bool xHeld {false};
        bool cHeld {false};
        bool vHeld {false};
        bool downHeld {false};
        bool leftHeld {false};
        bool rightHeld {false};
        bool upHeld {false};
        bool kp1Held {false};
        bool kp2Held {false};
        bool kp3Held {false};
        bool kp4Held {false};
        bool kp5Held {false};
        bool kp6Held {false};
        bool kp7Held {false};
        bool kp8Held {false};
        bool kp9Held {false};
        double mouseX {0.0};
        double mouseY {0.0};
        bool mouse1 {false};
        bool mouse2 {false};
        bool mouse3 {false};
        bool mouse4 {false};
        bool mouse5 {false};
        bool mouse6 {false};
        bool mouse7 {false};
        bool mouse8 {false};
};


class Window
{
    public:
        Window();
        Window(Config cfg);
        bool shouldRun() const;
        Input* getInputs() const;
        int focused() const;
        bool mouseLocked() const;
        void getCursorPos(double* p_X, double* p_Y);
        void update();
        void changeTitle(const std::string newTitle);
        void makeContextCurrent();
        inline GLFWwindow* getGLFWWindow() { return window; }

        virtual ~Window();

    protected:

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
        IndexBuffer(const std::vector<unsigned int>& data);
        IndexBuffer(const std::vector<unsigned short>& data);
        IndexBuffer(const std::vector<unsigned char>& data);
        IndexBuffer(const unsigned int data[], unsigned int size);
        IndexBuffer(const unsigned short data[], unsigned int size);
        IndexBuffer(const unsigned char data[], unsigned int size);
        void bindBuffer() const;
        void unbindBuffer() const;
        inline unsigned int getIboID() const { return iboID; }
        inline unsigned int getCount() const { return count; }
        inline unsigned int getDataType() const { return dataType; }
        void remove();
        ~IndexBuffer();

    protected:

    private:
        unsigned int iboID;
        unsigned int dataType;
        long unsigned int count;
};


class VertexBuffer
{
    public:
        VertexBuffer(const std::vector<float>& data);
        VertexBuffer(const float data[], unsigned int size);
        void bindBuffer() const;
        void unbindBuffer() const;
        inline unsigned int getVboID() const { return vboID; }
        void remove();
        ~VertexBuffer();

    protected:

    private:
        unsigned int vboID;
};


struct SourcePackage
{
    std::string vertex;
    std::string fragment;
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

        inline VertexBufferLayout getLayout() { return layout; }


    protected:

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
        ~VertexArray();

    protected:

    private:
        unsigned int vaoID;
};


class Texture
{
    public:
        Texture(const std::string& path, const unsigned int& slot, unsigned int minFilter = GL_NEAREST_MIPMAP_NEAREST, unsigned int magFilter = GL_NEAREST);
        Texture(unsigned char** image, const unsigned int& slot, unsigned int minFilter = GL_NEAREST_MIPMAP_NEAREST, unsigned int magFilter = GL_NEAREST);
        ~Texture();

        void bindTexture(unsigned int slot = 0) const;
        void unbindTexture() const;

        inline int getWidth() const {return width;}
        inline int getHeight() const {return height;}
        inline int getBPP() const {return bPP;}

    protected:

    private:
        unsigned int texID;
        std::string filePath;
        unsigned char* localBuffer;
        int width, height, bPP;
};


class Camera
{
    public:
        Camera(float x = 1.0f, float y = 1.0f, float z = 1.0f);
        glm::mat4 update(float deltaTime, Input inputs);
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
void drawTexture(const VertexArray& vao, const IndexBuffer& ibo, Shader& shader, int texSlot);


// Template vertices
std::vector<float> tVertsEquilateralXY(float posX, float posY, float size, bool centred = true);
std::vector<float> tVertsEquilateralXYUV(float posX, float posY, float size, bool centred = true);
std::vector<float> tVertsSquareXY(float posX, float posY, float size, bool centred = false);
std::vector<float> tVertsSquareXYUV(float posX, float posY, float size, bool centred = false);
std::vector<float> tVertsCubeXYZ(float posX, float posY, float posZ, float size, bool centred = false, bool shortened = true);
std::vector<float> tVertsCubeXYZUV(float posX, float posY, float posZ, float size, bool centred = false, bool shortened = true);

template <typename T>
inline std::vector<T> tIndsTriangles(T count)
{
    std::vector<T> result(3 * count);
    for (int i {0}; i < count; ++i)
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
    for (int i {0}; i < count; ++i)
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
        for (int i {0}; i < count; ++i)
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
        for (int i {0}; i < count; ++i)
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

std::vector<float> xyToXyz(const std::vector<float>& vertices2d, unsigned int vertexSize, float z = 1.0f);

}


#endif // PETROLEUM_H
