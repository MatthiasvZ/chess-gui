#include "other/ErrorFeedback.h"

#include <GL/glew.h>
#include <iostream>

void ErrorFeedback::clearErrors()
{
    while (glGetError());
}

void ErrorFeedback::getErrors()
{
    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << " / ";
        switch (error)
        {
        case 0x0500:
            std::cerr << "GL_INVALID_ENUM";
            break;
        case 0x0501:
            std::cerr << "GL_INVALID_VALUE";
            break;
        case 0x0502:
            std::cerr << "GL_INVALID_OPERATION";
            break;
        case 0x0503:
            std::cerr << "GL_STACK_OVERFLOW";
            break;
        case 0x0504:
            std::cerr << "GL_STACK_UNDERFLOW";
            break;
        case 0x0505:
            std::cerr << "GL_OUT_OF_MEMORY";
            break;
        default:
            std::cerr << "UNKNOWN";
            break;
        }
        std::cerr << ')' << std::endl;
    }
}
