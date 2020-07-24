//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GR_RENDERBUFFER_H
#define DOGBONE_GR_RENDERBUFFER_H

#include "GR_API.h"

#include <memory>
#include <string>
#include <vector>

namespace dogb::GR
{
enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static inline uint32_t
ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Int:
    case ShaderDataType::Float:
        return 4;
    case ShaderDataType::Int2:
    case ShaderDataType::Float2:
        return 8;
    case ShaderDataType::Int3:
    case ShaderDataType::Float3:
        return 12;
    case ShaderDataType::Int4:
    case ShaderDataType::Float4:
        return 16;
    case ShaderDataType::Mat3:
        return 36;
    case ShaderDataType::Mat4:
        return 64;
    case ShaderDataType::Bool:
        return 1;
    default:
        break;
    }

    return 0;
}

struct GR_API BufferElement
{
    BufferElement(
            ShaderDataType _type,
            const std::string &_name,
            bool _normalized = false)
        : name(_name)
        , type(_type)
        , size(ShaderDataTypeSize(_type))
        , offset(0)
        , is_normalized(_normalized)
    {
    }

    [[nodiscard]] uint32_t componentCount() const
    {
        switch (type)
        {
        case ShaderDataType::Int:
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Int2:
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Int3:
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Int4:
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 9;
        case ShaderDataType::Mat4:
            return 16;
        case ShaderDataType::Bool:
            return 1;
        default:
            break;
        }

        return 0;
    }

    std::string name;
    ShaderDataType type;
    uint32_t size;
    uint32_t offset;
    bool is_normalized;
};

class GR_API BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement> &elements)
        : m_elements(elements)
    {
        calculateOffsetsAndStride();
    }

    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const
    {
        return m_elements.begin();
    }
    [[nodiscard]] std::vector<BufferElement>::const_iterator end() const
    {
        return m_elements.end();
    }

    [[nodiscard]] inline const std::vector<BufferElement> &elements() const
    {
        return m_elements;
    }

    [[nodiscard]] inline uint32_t stride() const { return m_stride; }

private:
    void calculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        m_stride = 0;
        for (auto &&e : m_elements)
        {
            e.offset = offset;
            offset += e.size;
            m_stride += e.size;
        }
    }

    std::vector<BufferElement> m_elements;
    uint32_t m_stride{};
};

class GR_API VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    static std::unique_ptr<VertexBuffer> create(float *vertices, uint32_t size);
    static std::unique_ptr<VertexBuffer> create(uint32_t size);

    virtual void setData(const void* data, uint32_t size) = 0;

    virtual void setLayout(const BufferLayout &layout) = 0;
    [[nodiscard]] virtual const BufferLayout &bufferLayout() const = 0;
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

protected:
    VertexBuffer() = default;
};

class GR_API IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    static std::unique_ptr<IndexBuffer> create(uint32_t *indices, uint32_t size);

    [[nodiscard]] virtual uint32_t count() const = 0;
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

protected:
    IndexBuffer() = default;
};
} // namespace dogb::GR

#endif // DOGBONE_GR_RENDERBUFFER_H
