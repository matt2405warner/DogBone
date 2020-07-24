//
// Created by matt on 2020-07-12.
//

#ifndef DOGBONE_GR_OPENGLTEXTURE_H
#define DOGBONE_GR_OPENGLTEXTURE_H

#include "GR_API.h"

#include "GR_Texture.h"

#include <glad/glad.h>

namespace dogb::GR::OpenGL
{
class GR_API GLTexture2D : public Texture2D
{
public:
    explicit GLTexture2D(const std::string& path);
    GLTexture2D(uint32_t width, uint32_t height);
    ~GLTexture2D() override;

    void setData(void* data, uint32_t size) override;

    [[nodiscard]] U32 width() const override { return m_width; }
    [[nodiscard]] U32 height() const override { return m_height; }
    void bind(U32 slot = 0) const override;
    void unbind() const override ;

private:
    std::string m_path;
    U32 m_width;
    U32 m_height;
    U32 m_ID;
    GLenum m_format;
    GLenum m_internalFormat;
};
}

#endif // DOGBONE_GR_OPENGLTEXTURE_H
