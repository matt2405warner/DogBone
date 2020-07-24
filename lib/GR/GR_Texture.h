//
// Created by matt on 2020-07-12.
//

#ifndef DOGBONE_GR_TEXTURE_H
#define DOGBONE_GR_TEXTURE_H

#include "GR_API.h"

#include <CE/CE_Asset.h>

#include <SYS/SYS_Types.h>

#include <stb_image.h>

#include <memory>

namespace dogb::GR
{
class DB_GR_API Texture : public CE::Asset
    {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] virtual U32 width() const = 0;
        [[nodiscard]] virtual U32 height() const = 0;

        virtual void bind(U32 slot = 0) const = 0;
        virtual void unbind() const = 0;
        virtual void setData(void* data, uint32_t size) = 0;
    };

    class DB_GR_API Texture2D : public Texture
    {
    public:
        static std::unique_ptr<Texture2D> create(uint32_t width, uint32_t height);
        static std::unique_ptr<Texture2D> create(const std::string& path);
    };
}

#endif // DOGBONE_GR_TEXTURE_H
