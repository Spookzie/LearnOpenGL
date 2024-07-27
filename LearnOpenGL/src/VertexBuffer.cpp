#include "VertexBuffer.h"
#include "Renderer.h"


//Constructor
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glErrorCall( glGenBuffers(1, &m_RendererID) );       //Generating a buffer
    glErrorCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );      //Binding the buffer
    glErrorCall( glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW) );    //Updating vertex data
}

//Destructor
VertexBuffer::~VertexBuffer()
{
    glErrorCall( glDeleteBuffers(1, &m_RendererID) );
}


//Binding the buffers
void VertexBuffer::Bind() const
{
    glErrorCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );

}


//Unbinding the buffers
void VertexBuffer::Unbind() const
{
    glErrorCall( glBindBuffer(GL_ARRAY_BUFFER, 0) );

}