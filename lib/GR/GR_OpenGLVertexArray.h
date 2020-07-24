//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GR_OPENGLVERTEXARRAY_H
#define DOGBONE_GR_OPENGLVERTEXARRAY_H

#include "GR_API.h"

#include "GR_VertexArray.h"

#include <memory>
#include <vector>

namespace dogb::GR::OpenGL
{
class GR_API GLVertexArray : public VertexArray
{
public:
    GLVertexArray();
    ~GLVertexArray() override;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(std::shared_ptr<VertexBuffer> vbuffer) override;
    void setIndexBuffer(std::shared_ptr<IndexBuffer> ibuffer) override;

    [[nodiscard]] const std::vector<std::shared_ptr<VertexBuffer>> &vertexBuffers() const override
    {
        return m_vertexBuffers;
    }
    [[nodiscard]] std::shared_ptr<IndexBuffer> indexBuffer() const override { return m_indexBuffer; }

private:
    uint32_t m_rendererID;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
};
} // namespace dogb::GR::OpenGL

#endif // DOGBONE_GR_OPENGLVERTEXARRAY_H
