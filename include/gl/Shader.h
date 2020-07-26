#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>


struct SourcePackage
{
    std::string vertex;
    std::string fragment;
};

class Shader
{
    public:
        static std::string readFromFile(const char* filePath);

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


    protected:

    private:
        unsigned int m_Program;
        std::unordered_map<std::string, int> uniformLocationCache;
        unsigned int compileShader(const char* src, unsigned int type);
        int getUniformLocation(const std::string& name);
};

#endif // SHADER_H
