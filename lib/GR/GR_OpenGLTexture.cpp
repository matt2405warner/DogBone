//
// Created by matt on 2020-07-12.
//

#include "GR_OpenGLTexture.h"

#include <UT/UT_Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

namespace dogb::GR::OpenGL
{
GLTexture2D::GLTexture2D(uint32_t width, uint32_t height)
    : m_width(width), m_height(height), m_ID(0)
{
    m_internalFormat = GL_RGBA8;
    m_format = GL_RGBA;

    m_width = width;
    m_height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
    glTextureStorage2D(m_ID, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLTexture2D::GLTexture2D(const std::string &path)
    : m_path(path), m_width(0), m_height(0), m_ID(0)
{
    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    m_internalFormat = 0;
    m_format = 0;
    if (channels == 4)
    {
        m_internalFormat = GL_RGBA8;
        m_format = GL_RGBA;
    }
    else if (channels == 3)
    {
        m_internalFormat = GL_RGB8;
        m_format = GL_RGB;
    }

    m_width = static_cast<U32>(width);
    m_height = static_cast<U32>(height);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
    glTextureStorage2D(m_ID, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(
            m_ID, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE,
            data);

    stbi_image_free(data);
}

GLTexture2D::~GLTexture2D()
{
    glDeleteTextures(1, &m_ID);
}

void
GLTexture2D::bind(U32 slot) const
{
    glBindTextureUnit(slot, m_ID);
}

void
GLTexture2D::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
void
GLTexture2D::setData(void *data, uint32_t)
{
    glTextureSubImage2D(m_ID, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);
}

} // namespace dogb::GR::OpenGL