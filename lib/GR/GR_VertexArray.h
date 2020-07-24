//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_VERTEXARRAY_H
#define DOGBONE_VERTEXARRAY_H

#include "GR_API.h"

#include "GR_RenderBuffer.h"

#include <memory>

namespace dogb::GR
{
class GR_API VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(std::shared_ptr<VertexBuffer> vbuffer) = 0;
    virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> ibuffer) = 0;

    [[nodiscard]] virtual const std::vector<std::shared_ptr<VertexBuffer>> &vertexBuffers() const = 0;
    [[nodiscard]] virtual std::shared_ptr<IndexBuffer> indexBuffer() const = 0;

    static std::unique_ptr<VertexArray> create();

protected:
    VertexArray() = default;
};
} // namespace dogb::GR

#endif // DOGBONE_VERTEXARRAY_H
