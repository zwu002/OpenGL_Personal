#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool isDynamic) : m_isDynamic(isDynamic), m_data((float*)data)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    if (m_isDynamic)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }
    else
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    if (m_isDynamic)
    {
        // this is not working
        // GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_data), m_data));
    }
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}