//
// Created by matt on 2020-07-22.
//

#include "GR_SubTexture.h"

#include <utility>

namespace dogb::GR
{
SubTexture2D::SubTexture2D(
        std::shared_ptr<Texture2D> tex,
        const glm::vec2 &min,
        const glm::vec2 &max)
    : m_texture(std::move(tex))
{
    m_coords[0] = {min.x, min.y};
    m_coords[1] = {max.x, min.y};
    m_coords[2] = {max.x, max.y};
    m_coords[3] = {min.x, max.y};
}
std::shared_ptr<SubTexture2D>
SubTexture2D::createFromCoords(
        std::shared_ptr<Texture2D> tex,
        const glm::vec2 &coords,
        const glm::vec2 &cell_size,
        const glm::vec2 &size)
{
    const glm::vec2 min = {
            (coords.x * cell_size.x) / static_cast<float>(tex->width()),
            (coords.y * cell_size.y) / static_cast<float>(tex->height())};
    const glm::vec2 max = {
            ((coords.x + size.x) * cell_size.x) /
                    static_cast<float>(tex->width()),
            ((coords.y + size.y) * cell_size.y) /
                    static_cast<float>(tex->height())};

    return std::make_shared<SubTexture2D>(tex, min, max);
}

} // namespace dogb::GR
