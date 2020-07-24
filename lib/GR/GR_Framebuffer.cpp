//
// Created by matt on 2020-07-23.
//

#include "GR_Framebuffer.h"

#include "GR_Renderer.h"

#include "GR_OpenGLFramebuffer.h"

namespace dogb::GR
{
std::shared_ptr<Framebuffer>
dogb::GR::Framebuffer::create(const dogb::GR::Framebuffer::Specification &spec)
{
    switch (Renderer::api())
    {
    case Api::OpenGL:
        return std::make_unique<OpenGL::GLFramebuffer>(spec);
        break;
    default:
        break;
    }

    return nullptr;
}
}