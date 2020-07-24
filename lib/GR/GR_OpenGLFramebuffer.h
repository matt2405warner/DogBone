//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GR_OPENGLFRAMEBUFFER_H
#define DOGBONE_GR_OPENGLFRAMEBUFFER_H

#include "GR_API.h"

#include "GR_Framebuffer.h"

namespace dogb::GR::OpenGL
{
class DB_GR_API GLFramebuffer : public Framebuffer
{
public:
    explicit GLFramebuffer(const Specification& spec);
    ~GLFramebuffer() override;

    void invalidate();

    void resize(uint32_t width, uint32_t height) override ;

    [[nodiscard]] uint32_t colorAttachmentRendererID() const override { return m_colorAttachment; }

    void bind() override ;
    void unbind() override ;
private:
    uint32_t m_renderID{};
    uint32_t m_colorAttachment{};
    uint32_t m_depthAttachment{};
};
}

#endif // DOGBONE_GR_OPENGLFRAMEBUFFER_H
