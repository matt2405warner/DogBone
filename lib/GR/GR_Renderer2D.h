//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_RENDERER2D_H
#define DOGBONE_RENDERER2D_H

#include "GR_API.h"

#include "GR_OrthoGraphicCamera.h"
#include "GR_SubTexture.h"
#include "GR_Texture.h"

#include <glm/glm.hpp>

#include <memory>

#define DB_RENDER_NAME(_name_) static constexpr const char *_name_ = #_name_;

namespace dogb::GR
{
namespace RenderNames
{
DB_RENDER_NAME(u_TilingFactor)
DB_RENDER_NAME(u_Color)
DB_RENDER_NAME(u_Transform)
DB_RENDER_NAME(u_ViewProj)
DB_RENDER_NAME(u_Texture)
DB_RENDER_NAME(u_Textures)

DB_RENDER_NAME(a_Position)
DB_RENDER_NAME(a_Color)
DB_RENDER_NAME(a_TexCoord)
DB_RENDER_NAME(a_TexIndex)
DB_RENDER_NAME(a_TilingFactor)
} // namespace RenderNames

class GR_API Renderer2D
{
public:
    struct GR_API Statistics
    {
        uint32_t m_drawCalls = 0;
        uint32_t m_quadCount = 0;

        [[nodiscard]] uint32_t totalVertexCount() const { return m_quadCount * 4; }
        [[nodiscard]] uint32_t totalIndexCount() const { return m_quadCount * 6; }
    };

    static void init();
    static void shutdown();

    static void beginScene(const OrthoGraphicCamera &camera);
    static void endScene();

    static void flush();

    static void drawQuad(
            const glm::vec2 &position,
            const glm::vec2 &size,
            const glm::vec4 &color);
    static void drawQuad(
            const glm::vec3 &position,
            const glm::vec2 &size,
            const glm::vec4 &color);
    static void drawQuad(
            const glm::vec2 &position,
            const glm::vec2 &size,
            std::shared_ptr<Texture2D> tex,
            float tiling = 1.0f,
            const glm::vec4 &tint = glm::vec4(1.0f));
    static void drawQuad(
            const glm::vec3 &position,
            const glm::vec2 &size,
            std::shared_ptr<Texture2D> tex,
            float tiling = 1.0f,
            const glm::vec4 &tint = glm::vec4(1.0f));
    static void drawQuad(
            const glm::vec2 &position,
            const glm::vec2 &size,
            std::shared_ptr<SubTexture2D> tex,
            float tiling = 1.0f,
            const glm::vec4 &tint = glm::vec4(1.0f));
    static void drawQuad(
            const glm::vec3 &position,
            const glm::vec2 &size,
            std::shared_ptr<SubTexture2D> tex,
            float tiling = 1.0f,
            const glm::vec4 &tint = glm::vec4(1.0f));

    static void drawRotatedQuad(
            const glm::vec2 &position,
            const glm::vec2 &size,
            float rotation,
            const glm::vec4 &color);
    static void drawRotatedQuad(
            const glm::vec3 &position,
            const glm::vec2 &size,
            float rotation,
            const glm::vec4 &color);
    static void drawRotatedQuad(
            const glm::vec2 &position,
            const glm::vec2 &size,
            float rotation,
            std::shared_ptr<Texture2D> tex,
            float tiling = 1.0f,
            const glm::vec4 &tint = glm::vec4(1.0f));
    static void drawRotatedQuad(
            const glm::vec3 &position,
            const glm::vec2 &size,
            float rotation,
            std::shared_ptr<Texture2D> tex,
            float tiling = 1.0f,
            const glm::vec4 &tint = glm::vec4(1.0f));
    static void drawRotatedQuad(
            const glm::vec2 &position,
            const glm::vec2 &size,
            float rotation,
            std::shared_ptr<SubTexture2D> tex,
            float tiling = 1.0f,
            const glm::vec4 &tint = glm::vec4(1.0f));
    static void drawRotatedQuad(
            const glm::vec3 &position,
            const glm::vec2 &size,
            float rotation,
            std::shared_ptr<SubTexture2D> tex,
            float tiling = 1.0f,
            const glm::vec4 &tint = glm::vec4(1.0f));

    static void resetStats();
    static const Statistics& statistics();
};
} // namespace dogb::GR

#endif // DOGBONE_RENDERER2D_H
