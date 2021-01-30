//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GR_FRAMEBUFFER_H
#define DOGBONE_GR_FRAMEBUFFER_H

#include "GR_API.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace dogb::GR
{
class DB_GR_API Framebuffer
{
public:
    enum class TextureFormat
    {
        None = 0,
        // Color
        RGBA8,
        RED_INTEGER,
        // Depth/stencil
        DEPTH24STENCIL8,
        // Defaults
        Depth = DEPTH24STENCIL8
    };

    struct DB_GR_API TextureSpecification
    {
        TextureSpecification() = default;
        TextureSpecification(TextureFormat format) :
            m_format(format)
        {}

        TextureFormat m_format = TextureFormat::None;
        // TODO: add filtering/wrapping
    };

    struct DB_GR_API AttachmentSpecification
    {
        AttachmentSpecification() = default;
        AttachmentSpecification(std::initializer_list<TextureSpecification> l) :
            m_attachments(l)
        {}

        std::vector<TextureSpecification> m_attachments;
    };

    struct DB_GR_API Specification
    {
        uint32_t m_width = 0;
        uint32_t m_height = 0;
        uint32_t m_samples = 1;
        bool m_swapChainTarget = false;
        AttachmentSpecification m_attachments;
    };

    explicit Framebuffer(const Specification& spec) :
        m_specification(spec)
    {}
    virtual ~Framebuffer() = default;

    static std::shared_ptr<Framebuffer> create(const Specification& spec);

    virtual void resize(uint32_t width, uint32_t height) = 0;

    [[nodiscard]] const Specification& specification() const { return m_specification; }
    Specification& specification() { return m_specification; }

    [[nodiscard]] virtual uint32_t colorAttachmentRendererID(uint32_t index = 0) const = 0;
    [[nodiscard]] virtual int readPixel(uint32_t attachement_index, int x, int y) const = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;
protected:
    Specification m_specification;
};
}

#endif // DOGBONE_GR_FRAMEBUFFER_H
