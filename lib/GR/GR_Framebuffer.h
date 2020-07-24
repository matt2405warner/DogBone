//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GR_FRAMEBUFFER_H
#define DOGBONE_GR_FRAMEBUFFER_H

#include "GR_API.h"

#include <glm/glm.hpp>

#include <memory>

namespace dogb::GR
{
class GR_API Framebuffer
{
public:
    struct GR_API Specification
    {
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_samples = 1;
        bool m_swapChainTarget = false;
    };

    explicit Framebuffer(const Specification& spec) :
        m_specification(spec)
    {}
    virtual ~Framebuffer() = default;

    static std::shared_ptr<Framebuffer> create(const Specification& spec);

    virtual void resize(uint32_t width, uint32_t height) = 0;

    [[nodiscard]] const Specification& specification() const { return m_specification; }
    Specification& specification() { return m_specification; }

    [[nodiscard]] virtual uint32_t colorAttachmentRendererID() const = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;
protected:
    Specification m_specification;
};
}

#endif // DOGBONE_GR_FRAMEBUFFER_H
