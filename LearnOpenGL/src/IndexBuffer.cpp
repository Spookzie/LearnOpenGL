#include "IndexBuffer.h"
#include "Renderer.h"


//Constructor
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : count(count)
{
    glErrorCall( glGenBuffers(1, &rendererID) );       //Generating a buffer
    glErrorCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID) );      //Binding the buffer
    glErrorCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));    //Updating vertex data
}

//Destructor
IndexBuffer::~IndexBuffer()
{
    glErrorCall( glDeleteBuffers(1, &rendererID) );
}


//Binding the buffers
void IndexBuffer::Bind() const
{
    glErrorCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID) );

}


//Unbinding the buffers
void IndexBuffer::Unbind() const
{
    glErrorCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );

}