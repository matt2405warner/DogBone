//
// Created by matt on 2020-07-23.
//

#include "GR_OpenGLFramebuffer.h"

#include <UT/UT_Logger.h>

#include <UT/UT_Assert.h>
#include <glad/glad.h>

namespace dogb::GR::OpenGL
{
static uint32_t theMaxFramebufferSize = 8192;

static bool
gr_isDepthFormat(Framebuffer::TextureFormat format)
{
    switch (format)
    {
    case Framebuffer::TextureFormat::DEPTH24STENCIL8:
        return true;
    default:
        return false;
    }
}

static GLenum
gr_textureTarget(bool multisampled)
{
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void
gr_bindTexture(bool multisampled, uint32_t id)
{
    glBindTexture(gr_textureTarget(multisampled), id);
}

static void
gr_createTextures(bool multisample, uint32_t *out_id, uint32_t count)
{
    glCreateTextures(gr_textureTarget(multisample), count, out_id);
}

static void
gr_attachColorTexture(
        uint32_t id,
        uint32_t samples,
        GLenum internal_format,
        GLenum format,
        uint32_t width,
        uint32_t height,
        uint32_t index)
{
    bool multisampled = samples > 1;
    if (multisampled)
    {
        glTexImage2DMultisample(
                GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height,
                GL_FALSE);
    }
    else
    {
        glTexImage2D(
                GL_TEXTURE_2D, 0, internal_format, width, height, 0, format,
                GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
            gr_textureTarget(multisampled), id, 0);
}

static void
gr_attachDepthTexture(
                uint32_t id,
                uint32_t samples,
                GLenum format,
                GLenum attachment,
                uint32_t width,
                uint32_t height
                )
{
    bool multisampled = samples > 1;
    if (multisampled)
    {
        glTexImage2DMultisample(
                GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height,
                GL_FALSE);
    }
    else
    {
        glTexStorage2D(
                GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(
            GL_FRAMEBUFFER, attachment,
            gr_textureTarget(multisampled), id, 0);
}

GLFramebuffer::GLFramebuffer(const Specification &spec) : Framebuffer(spec)
{
    for (auto format : m_specification.m_attachments.m_attachments)
    {
        if (!gr_isDepthFormat(format.m_format))
            m_colorAttachmentSpecs.emplace_back(format);
        else
            m_depthAttachmentSpec = format;
    }

    invalidate();
}

GLFramebuffer::~GLFramebuffer()
{
    clear_();
}

void
GLFramebuffer::clear_()
{
    if (m_renderID > 0)
    {
        glDeleteFramebuffers(1, &m_renderID);
        glDeleteTextures((GLsizei)m_colorAttachments.size(), m_colorAttachments.data());
        glDeleteTextures(1, &m_depthAttachment);
        m_colorAttachments.clear();
        m_depthAttachment = 0;
    }
    m_renderID = 0;
}

void
GLFramebuffer::invalidate()
{
    clear_();

    glCreateFramebuffers(1, &m_renderID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);

    bool multisample = m_specification.m_samples > 1;
    // -------------------------------------------------------------------------
    // Attachments
    // -------------------------------------------------------------------------
    if (!m_colorAttachmentSpecs.empty())
    {
        m_colorAttachments.resize(m_colorAttachmentSpecs.size());
        gr_createTextures(
                multisample, m_colorAttachments.data(),
                (uint32_t)m_colorAttachments.size());

        for (uint32_t i = 0; i < m_colorAttachments.size(); i++)
        {
            gr_bindTexture(multisample, m_colorAttachments[i]);
            switch (m_colorAttachmentSpecs[i].m_format)
            {
            case GR::Framebuffer::TextureFormat::RGBA8:
                gr_attachColorTexture(
                        m_colorAttachments[i], m_specification.m_samples,
                        GL_RGBA8,
                        GL_RGBA, m_specification.m_width,
                        m_specification.m_height, i);
                break;
            case GR::Framebuffer::TextureFormat::RED_INTEGER:
                gr_attachColorTexture(
                        m_colorAttachments[i],
                        m_specification.m_samples,
                        GL_R32I, GL_RED_INTEGER,
                        m_specification.m_width,
                        m_specification.m_height,
                        i
                        );
            default:
                break;
            }
        }
    }

    // -------------------------------------------------------------------------
    // Depth Attachment
    // -------------------------------------------------------------------------
    if (m_depthAttachmentSpec.m_format != Framebuffer::TextureFormat::None)
    {
        gr_createTextures(multisample, &m_depthAttachment, 1);
        gr_bindTexture(multisample, m_depthAttachment);
        switch (m_depthAttachmentSpec.m_format)
        {
        case Framebuffer::TextureFormat::DEPTH24STENCIL8:
            gr_attachDepthTexture(
                    m_depthAttachment, m_specification.m_samples,
                    GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                    m_specification.m_width, m_specification.m_height);
            break;
        default:
            break;
        }
    }

    if (m_colorAttachments.size() > 1)
    {
        UT_ASSERT(m_colorAttachments.size() <= 4);

        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers((GLsizei)m_colorAttachments.size(), buffers);
    }
    else if (m_colorAttachments.empty())
    {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        UT_LOG_ERROR("Frame buffer creation failed...");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void
GLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);
    glViewport(0, 0, m_specification.m_width, m_specification.m_height);
}
void
GLFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void
GLFramebuffer::resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > theMaxFramebufferSize ||
        height > theMaxFramebufferSize)
        return;

    m_specification.m_width = width;
    m_specification.m_height = height;

    invalidate();
}
int
GLFramebuffer::readPixel(uint32_t attachement_index, int x, int y) const
{
    UT_ASSERT(attachement_index < m_colorAttachments.size());
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachement_index);
    int pixel = 0;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);
    return pixel;
}

} // namespace dogb::GR::OpenGL
