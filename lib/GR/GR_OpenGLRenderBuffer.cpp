//
// Created by matt on 2020-07-11.
//

#include "GR_OpenGLRenderBuffer.h"

#include <glad/glad.h>

namespace dogb::GR::OpenGL
{
// -----------------------------------------------------------------------------
// VertexBuffer ----------------------------------------------------------------
// -----------------------------------------------------------------------------

GLVertexBuffer::GLVertexBuffer(float *vertices, uint32_t size) :
    m_rendererID(0)
{
    glCreateBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

GLVertexBuffer::GLVertexBuffer(uint32_t size) :
    m_rendererID(0)
{
    glCreateBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &m_rendererID);
}

void
GLVertexBuffer::setData(const void *data, uint32_t size)
{
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void
GLVertexBuffer::setLayout(const BufferLayout &layout)
{
    m_layout = layout;
}

void
GLVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void
GLVertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// -----------------------------------------------------------------------------
// IndexBuffer -----------------------------------------------------------------
// -----------------------------------------------------------------------------

GLIndexBuffer::GLIndexBuffer(uint32_t *indices, uint32_t count) : m_count(count)
{
    glCreateBuffers(1, &m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)),
            indices, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &m_rendererID);
}

void
GLIndexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void
GLIndexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace dogb::GR::OpenGL