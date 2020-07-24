//
// Created by matt on 2020-07-11.
//

#include "GR_VertexArray.h"

#include "GR_OpenGLVertexArray.h"
#include "GR_Renderer.h"

namespace dogb::GR
{
std::unique_ptr<VertexArray>
VertexArray::create()
{
    switch (Renderer::api())
    {
    case Api::OpenGL:
        return std::make_unique<OpenGL::GLVertexArray>();
    default:
        break;
    }
    return nullptr;
}
} // namespace dogb::GR