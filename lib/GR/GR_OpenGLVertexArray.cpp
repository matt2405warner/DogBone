//
// Created by matt on 2020-07-11.
//

#include "GR_OpenGLVertexArray.h"

#include <glad/glad.h>

namespace dogb::GR::OpenGL
{
static inline GLenum
ShaderDataTypeToOpenGLBaseType(GR::ShaderDataType type)
{
    using namespace GR;
    switch (type)
    {
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    default:
        break;
    }

    return 0;
}

GLVertexArray::GLVertexArray() : m_rendererID(0)
{
    glCreateVertexArrays(1, &m_rendererID);
}
GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &m_rendererID);
}

void
GLVertexArray::bind() const
{
    glBindVertexArray(m_rendererID);
}
void
GLVertexArray::unbind() const
{
    glBindVertexArray(0);
}

void
GLVertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer> vbuffer)
{
    if (!vbuffer)
        return;

    bind();

    vbuffer->bind();
    const BufferLayout &layout = vbuffer->bufferLayout();
    uint32_t index = 0;
    for (auto &&e : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
                index, static_cast<int>(e.componentCount()), ShaderDataTypeToOpenGLBaseType(e.type),
                e.is_normalized ? GL_TRUE : GL_FALSE, static_cast<int>(layout.stride()),
                reinterpret_cast<const void *>(e.offset));
        index++;
    }

    m_vertexBuffers.emplace_back(vbuffer);
}
void
GLVertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer> ibuffer)
{
    bind();

    ibuffer->bind();

    m_indexBuffer = ibuffer;
}
} // namespace dogb::GR::OpenGL