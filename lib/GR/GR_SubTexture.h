//
// Created by matt on 2020-07-22.
//

#ifndef DOGBONE_GR_SUBTEXTURE_H
#define DOGBONE_GR_SUBTEXTURE_H

#include "GR_API.h"

#include "GR_Texture.h"

#include <glm/glm.hpp>

#include <memory>

namespace dogb::GR
{
class SubTexture2D
{
public:
    SubTexture2D(
            std::shared_ptr<Texture2D> tex,
            const glm::vec2 &min,
            const glm::vec2 &max);

    [[nodiscard]] std::shared_ptr<Texture2D> texture() const
    {
        return m_texture;
    }
    [[nodiscard]] const glm::vec2 *texCoords() const { return m_coords; }

    static std::shared_ptr<SubTexture2D> createFromCoords(
            std::shared_ptr<Texture2D> tex,
            const glm::vec2 &coords,
            const glm::vec2 &cell_size,
            const glm::vec2 &size = {1, 1});

private:
    std::shared_ptr<Texture2D> m_texture;
    glm::vec2 m_coords[4];
};
} // namespace dogb::GR

#endif // DOGBONE_GR_SUBTEXTURE_H
