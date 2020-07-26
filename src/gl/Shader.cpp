#include "gl/Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>


std::string Shader::readFromFile(const char* filePath)
{
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    fprintf(stderr, "Couldn't read in shader file '%s'\n", filePath);
    return "";
}

Shader::Shader(SourcePackage srcpkg)
{
    m_Program = glCreateProgram();
    unsigned int vertexShader = compileShader(srcpkg.vertex.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(srcpkg.fragment.c_str(), GL_FRAGMENT_SHADER);

    glAttachShader(m_Program, vertexShader);
    glAttachShader(m_Program, fragmentShader);
    glLinkProgram(m_Program);
    glValidateProgram(m_Program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int Shader::compileShader(const char* src, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        std::cerr << "Error: Couldn't compile " << \
                (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << \
                "Shader!\n";
        char buffer[1024];
        glGetShaderInfoLog(id, 1024, NULL, buffer);
        printf("%s", buffer);
    }
    return id;
}

void Shader::bindShader() const
{
    glUseProgram(m_Program);
}

void Shader::unbindShader() const
{
    glUseProgram(GL_FALSE);
}


void Shader::setUniform1i(const std::string& name, int v0)
{
    glUniform1i(getUniformLocation(name), v0);
}

void Shader::setUniform2i(const std::string& name, int v0, int v1)
{
    glUniform2i(getUniformLocation(name), v0, v1);
}

void Shader::setUniform3i(const std::string& name, int v0, int v1, int v2)
{
    glUniform3i(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4i(const std::string& name, int v0, int v1, int v2, int v3)
{
    glUniform4i(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform1f(const std::string& name, float v0)
{
    glUniform1f(getUniformLocation(name), v0);
}

void Shader::setUniform2f(const std::string& name, float v0, float v1)
{
    glUniform2f(getUniformLocation(name), v0, v1);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat2f(const std::string& name, const glm::mat2& mat)
{
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniformMat3f(const std::string& name, const glm::mat3& mat)
{
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

int Shader::getUniformLocation(const std::string& name)
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    int location = glGetUniformLocation(m_Program, name.c_str());
    #ifdef DEBUG
    if (location == -1)
        std::cerr << "Warning: uniform '" << name << "' does not exist!" << std::endl;
    #endif
    uniformLocationCache[name] = location;
    return location;
}

Shader::~Shader()
{
    glDeleteShader(m_Program);
}
