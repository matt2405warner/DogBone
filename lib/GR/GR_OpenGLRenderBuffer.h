//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GR_OPENGLRENDERBUFFER_H
#define DOGBONE_GR_OPENGLRENDERBUFFER_H

#include "GR_API.h"

#include "GR_RenderBuffer.h"

namespace dogb::GR::OpenGL
{
class DB_GR_API GLVertexBuffer : public VertexBuffer
{
public:
    GLVertexBuffer(float *vertices, uint32_t size);
    explicit GLVertexBuffer(uint32_t size);
    ~GLVertexBuffer() override;

    void setData(const void* data, uint32_t size) override ;
    void setLayout(const BufferLayout &layout) override;
    [[nodiscard]] const BufferLayout &bufferLayout() const override { return m_layout; }
    void bind() const override;
    void unbind() const override;

private:
    uint32_t m_rendererID;
    BufferLayout m_layout;
};

class DB_GR_API GLIndexBuffer : public IndexBuffer
{
public:
    GLIndexBuffer(uint32_t *vertices, uint32_t size);
    ~GLIndexBuffer() override;

    [[nodiscard]] uint32_t count() const override { return m_count; }
    void bind() const override;
    void unbind() const override;

private:
    uint32_t m_rendererID;
    uint32_t m_count;
};
} // namespace dogb::GR::OpenGL

#endif // DOGBONE_GR_OPENGLRENDERBUFFER_H
