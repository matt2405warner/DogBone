//
// Created by matt on 2020-07-12.
//

#include "GR_Texture.h"

#include "GR_OpenGLTexture.h"

namespace dogb::GR
{
std::unique_ptr<Texture2D>
Texture2D::create(const std::string &path) {
    return std::make_unique<OpenGL::GLTexture2D>(path);
}
std::unique_ptr<Texture2D>
Texture2D::create(uint32_t width, uint32_t height)
{
    return std::make_unique<OpenGL::GLTexture2D>(width, height);
}
}