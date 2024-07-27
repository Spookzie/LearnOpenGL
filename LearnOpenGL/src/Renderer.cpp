#include "Renderer.h"

#include <iostream>


const char* GetGLErrorString(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:
        return "Unknown Error";
    }
}


//Clearing previous errors
void glClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}


//Checking for new errors
bool glLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        const char* errorString = GetGLErrorString(error);
        std::cout << "ERROR::Type: " << errorString << "\n\tFunction: " << function <<
            "\n\tFile: " << file << "\n\tLine: " << line << std::endl;

        return false;
        /*
        If error is present, the loop returns false.
        This works with the same concept of break and is used as a replacement because we needed a boolean function
        */
    }

    return true;
}