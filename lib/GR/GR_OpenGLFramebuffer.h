//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GR_OPENGLFRAMEBUFFER_H
#define DOGBONE_GR_OPENGLFRAMEBUFFER_H

#include "GR_API.h"

#include "GR_Framebuffer.h"

#include <UT/UT_Assert.h>

namespace dogb::GR::OpenGL
{
class DB_GR_API GLFramebuffer : public Framebuffer
{
public:
    explicit GLFramebuffer(const Specification& spec);
    ~GLFramebuffer() override;

    void invalidate();

    void resize(uint32_t width, uint32_t height) override ;

    [[nodiscard]] uint32_t colorAttachmentRendererID(uint32_t index) const override
    {
        UT_ASSERT(index < m_colorAttachments.size());
        return m_colorAttachments[index];
    }
    int readPixel(uint32_t attachement_index, int x, int y) const override;

    void bind() override ;
    void unbind() override ;
private:
    void clear_();

    uint32_t m_renderID{};
    std::vector<Framebuffer::TextureSpecification> m_colorAttachmentSpecs;
    Framebuffer::TextureSpecification m_depthAttachmentSpec;

    std::vector<uint32_t> m_colorAttachments;
    uint32_t m_depthAttachment{};
};
}

#endif // DOGBONE_GR_OPENGLFRAMEBUFFER_H
