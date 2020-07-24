//
// Created by matt on 2020-07-11.
//

#include "GR_RenderBuffer.h"

#include "GR_OpenGLRenderBuffer.h"
#include "GR_Renderer.h"

namespace dogb::GR
{
std::unique_ptr<VertexBuffer>
VertexBuffer::create(float* vertices, uint32_t size)
{
    switch(Renderer::api())
    {
    case Api::OpenGL:
        return  std::make_unique<OpenGL::GLVertexBuffer>(vertices, size);
        break;
    default:
        break;
    }

    return nullptr;
}
std::unique_ptr<VertexBuffer>
VertexBuffer::create(uint32_t size)
{
    switch(Renderer::api())
    {
    case Api::OpenGL:
        return  std::make_unique<OpenGL::GLVertexBuffer>(size);
        break;
    default:
        break;
    }

    return nullptr;
}

std::unique_ptr<IndexBuffer>
IndexBuffer::create(uint32_t* indices, uint32_t size)
{
    switch(Renderer::api())
    {
    case Api::OpenGL:
        return std::make_unique<OpenGL::GLIndexBuffer>(indices, size);
        break;
    default:
        break;
    }

    return nullptr;
}

}