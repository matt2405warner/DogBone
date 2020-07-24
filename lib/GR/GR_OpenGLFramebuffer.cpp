//
// Created by matt on 2020-07-23.
//

#include "GR_OpenGLFramebuffer.h"

#include <UT/UT_Logger.h>

#include <glad/glad.h>

namespace dogb::GR::OpenGL
{
static uint32_t theMaxFramebufferSize = 8192;

GLFramebuffer::GLFramebuffer(const Specification &spec) : Framebuffer(spec)
{
    invalidate();
}

GLFramebuffer::~GLFramebuffer()
{
    if (m_renderID > 0)
    {
        glDeleteFramebuffers(1, &m_renderID);
        glDeleteTextures(1, &m_colorAttachment);
        glDeleteTextures(1, &m_depthAttachment);
    }
    m_renderID = 0;
}

void
GLFramebuffer::invalidate()
{
    if (m_renderID > 0)
    {
        glDeleteFramebuffers(1, &m_renderID);
        glDeleteTextures(1, &m_colorAttachment);
        glDeleteTextures(1, &m_depthAttachment);
    }

    glCreateFramebuffers(1, &m_renderID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);

    // -------------------------------------------------------------------------
    // Color Attachment
    // -------------------------------------------------------------------------
    glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_colorAttachment);

    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.m_width,
            m_specification.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            m_colorAttachment, 0);

    // -------------------------------------------------------------------------
    // Depth Attachment
    // -------------------------------------------------------------------------

    glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_depthAttachment);

#if 1
    glTexStorage2D(
            GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_specification.m_width,
            m_specification.m_height);
#else
    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specification.m_width,
            m_specification.m_height, 0, GL_DEPTH24_STENCIL8,
            GL_UNSIGNED_INT_24_8, nullptr);
#endif

    glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
            m_depthAttachment, 0);

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

} // namespace dogb::GR::OpenGL
