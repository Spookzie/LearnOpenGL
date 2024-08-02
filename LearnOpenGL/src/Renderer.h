#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

 
//  MACROS  //
#define ASSERT(x) if(!(x)) __debugbreak();  //Add a break point at the line where error occured
#define glErrorCall(x) glClearErrors();\
    x;\
    ASSERT(glLogCall(#x, __FILE__, __LINE__))


//  Error Handling  //
const char* GetGLErrorString(GLenum error);
void glClearErrors();
bool glLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};