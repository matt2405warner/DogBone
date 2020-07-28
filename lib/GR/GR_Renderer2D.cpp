//
// Created by matt on 2020-07-14.
//

#include "GR_Renderer2D.h"

#include "GR_Renderer.h"
#include "GR_Shader.h"
#include "GR_VertexArray.h"

#include <CE/CE_Asset.h>

#include <UT/UT_Profiler.h>

#include <glm/gtc/matrix_transform.hpp>
#include <utility>

namespace dogb::GR
{
struct QuadVertex
{
    glm::vec3 m_position;
    glm::vec4 m_color;
    glm::vec2 m_texCoord;
    float m_texIndex;
    float m_tilingFactor;
};

struct RenderStorage
{
    // Max values for 1 draw call.
    static const uint32_t theMaxQuads = 20000;
    static const uint32_t theMaxVertices = theMaxQuads * 4;
    static const uint32_t theMaxIndices = theMaxQuads * 6;
    static const uint32_t theMaxTextureSlots =
            32; // TODO: render caps (make it configurable)

    void init()
    {
        UT_PROFILE_FUNCTION();

        m_VAO = GR::VertexArray::create();
        m_VB = GR::VertexBuffer::create(theMaxVertices * sizeof(QuadVertex));

        GR::BufferLayout quad_layout{
                {GR::ShaderDataType::Float3, RenderNames::a_Position},
                {GR::ShaderDataType::Float4, RenderNames::a_Color},
                {GR::ShaderDataType::Float2, RenderNames::a_TexCoord},
                {GR::ShaderDataType::Float, RenderNames::a_TexIndex},
                {GR::ShaderDataType::Float, RenderNames::a_TilingFactor}};
        m_VB->setLayout(quad_layout);
        m_VAO->addVertexBuffer(m_VB);

        std::array<uint32_t, theMaxIndices> quad_indices{};

        uint32_t offset = 0;
        for (uint32_t i = 0; i < theMaxIndices; i += 6)
        {
            quad_indices[i] = offset + 0;
            quad_indices[i + 1] = offset + 1;
            quad_indices[i + 2] = offset + 2;

            quad_indices[i + 3] = offset + 2;
            quad_indices[i + 4] = offset + 3;
            quad_indices[i + 5] = offset + 0;

            offset += 4;
        }
        std::shared_ptr<GR::IndexBuffer> quad_ibuffer(
                GR::IndexBuffer::create(quad_indices.data(), theMaxIndices));
        m_VAO->setIndexBuffer(quad_ibuffer);

        m_whiteTexture = Texture2D::create(1, 1);
        uint32_t tex_data = 0xffffffff;
        m_whiteTexture->setData(&tex_data, sizeof(uint32_t));
        m_textureSlots[0] = m_whiteTexture;

        int32_t samplers[theMaxTextureSlots];
        for (int32_t i = 0; i < static_cast<int32_t>(theMaxTextureSlots); i++)
            samplers[i] = i;
        m_texShader = GR::Shader::create("../assets/shaders/Texture.glsl");
        m_texShader->bind();
        m_texShader->setIntv(
                RenderNames::u_Textures, samplers, theMaxTextureSlots);

        m_quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        m_quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        m_quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        m_quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    }

    void shutdown()
    {
        m_VAO.reset();
        m_texShader.reset();
        m_whiteTexture.reset();
        m_VB.reset();

        for (auto &slot : m_textureSlots)
            slot.reset();
    }

    std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VB;
    std::shared_ptr<Shader> m_texShader;
    std::shared_ptr<Texture2D> m_whiteTexture;
    std::array<QuadVertex, theMaxVertices> m_vertices;
    uint32_t m_quadVertexCount = 0;
    uint32_t m_quadIndexCount = 0;
    std::array<std::shared_ptr<Texture>, theMaxTextureSlots> m_textureSlots;
    // index 0 is used for the white texture
    uint32_t m_textureSlotIndex = 1;
    glm::vec4 m_quadVertexPositions[4];

    Renderer2D::Statistics m_stats;
};

static RenderStorage theRenderStorage;

void
Renderer2D::init()
{
    theRenderStorage.init();
}

void
Renderer2D::shutdown()
{
    theRenderStorage.shutdown();
}

static void
grResetBatchInfo()
{
    theRenderStorage.m_quadVertexCount = 0;
    theRenderStorage.m_quadIndexCount = 0;
    theRenderStorage.m_textureSlotIndex = 1;
}

static void
grFlushAndReset()
{
    Renderer2D::endScene();
    grResetBatchInfo();
}

void
Renderer2D::beginScene(const Camera &camera)
{
    glm::mat4 proj, view;
    camera.calculateMatricies(proj, view);
    glm::mat4 viewproj = proj * view;

    theRenderStorage.m_texShader->bind();
    theRenderStorage.m_texShader->setMat4(
            RenderNames::u_ViewProj, viewproj);

    grResetBatchInfo();
}
void
Renderer2D::endScene()
{
    UT_PROFILE_FUNCTION();

    // If we dont have anything to render then dont render anything.
    if (theRenderStorage.m_quadVertexCount > 0)
    {
        theRenderStorage.m_VB->setData(
                theRenderStorage.m_vertices.data(),
                theRenderStorage.m_quadVertexCount *
                        static_cast<uint32_t>(sizeof(QuadVertex)));

        flush();
    }

    // Clear out the shared pointers as we no longer need them. Make sure that
    // we dont clear the white texture
    for (uint32_t i = 1; i < theRenderStorage.m_textureSlotIndex; i++)
        theRenderStorage.m_textureSlots[i].reset();
}

void
Renderer2D::flush()
{
    // If we dont have anything to flush dont flush anything.
    if (theRenderStorage.m_quadVertexCount <= 0)
        return;

    // Bind all of our textures
    for (uint32_t i = 0; i < theRenderStorage.m_textureSlotIndex; i++)
        theRenderStorage.m_textureSlots[i]->bind(i);

    Renderer::drawIndexed(
            theRenderStorage.m_VAO, theRenderStorage.m_quadIndexCount);

    for (uint32_t i = 0; i < theRenderStorage.m_textureSlotIndex; i++)
        theRenderStorage.m_textureSlots[i]->unbind();

    grResetBatchInfo();

    theRenderStorage.m_stats.m_drawCalls++;
}

static glm::mat4
grCalcTransform(
        const glm::vec3 &position,
        const glm::vec2 &size,
        float rotation = 0.0f)
{
    return glm::translate(glm::mat4(1.0f), position) *
           glm::rotate(
                   glm::mat4(1.0f), glm::radians(rotation),
                   {0.0f, 0.0f, 1.0f}) *
           glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
}

static void
grSubmitRender(
        const glm::mat4 &transform,
        const glm::vec4 &color,
        std::shared_ptr<Texture> tex = nullptr,
        float tiling = 1.0f,
        const glm::vec2 coords[4] = nullptr)
{
    UT_PROFILE_FUNCTION()

    // NB: make sure to flush the scene if we have reached the max vertices for
    //      one draw call.
    if (theRenderStorage.m_quadIndexCount >= RenderStorage::theMaxIndices)
        grFlushAndReset();

    bool use_texture = tex != nullptr;
    float tex_slot = 0;
    if (use_texture)
    {
        // Check if we already have this texture bound to a slot.
        CE::UUID tex_id = tex->id();
        for (uint32_t i = 1; i < theRenderStorage.m_textureSlotIndex; i++)
        {
            if (theRenderStorage.m_textureSlots[i]->id() == tex_id)
            {
                tex_slot = static_cast<float>(i);
                break;
            }
        }

        // Check if we did not find a slot.
        if (tex_slot == 0.0f)
        {
            // We are already at our max texture slots. We cannot render this
            // quad with the correct texture.
            // TODO: instead of just failing use an ugly texture to indicate
            // something
            //      went terribly wrong.
            if (theRenderStorage.m_textureSlotIndex >=
                RenderStorage::theMaxTextureSlots)
                return;

            tex_slot = static_cast<float>(theRenderStorage.m_textureSlotIndex);
            theRenderStorage
                    .m_textureSlots[theRenderStorage.m_textureSlotIndex] = tex;
            theRenderStorage.m_textureSlotIndex++;
        }
    }

    // x1y1
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_position =
            transform * theRenderStorage.m_quadVertexPositions[0];
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_color =
            color;
    if (coords == nullptr)
    {
        theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
                .m_texCoord = {0.0f, 0.0f};
    }
    else
    {
        theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
                .m_texCoord = coords[0];
    }
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_texIndex =
            tex_slot;
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
            .m_tilingFactor = tiling;
    theRenderStorage.m_quadVertexCount++;
    // x2y1
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_position =
            transform * theRenderStorage.m_quadVertexPositions[1];
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_color =
            color;
    if (coords == nullptr)
    {
        theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
                .m_texCoord = {1.0f, 0.0f};
    }
    else
    {
        theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
                .m_texCoord = coords[1];
    }
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_texIndex =
            tex_slot;
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
            .m_tilingFactor = tiling;
    theRenderStorage.m_quadVertexCount++;
    // x2y2
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_position =
            transform * theRenderStorage.m_quadVertexPositions[2];
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_color =
            color;
    if (coords == nullptr)
    {
        theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
                .m_texCoord = {1.0f, 1.0f};
    }
    else
    {
        theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
                .m_texCoord = coords[2];
    }
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_texIndex =
            tex_slot;
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
            .m_tilingFactor = tiling;
    theRenderStorage.m_quadVertexCount++;
    // x1y2
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_position =
            transform * theRenderStorage.m_quadVertexPositions[3];
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_color =
            color;
    if (coords == nullptr)
    {
        theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
                .m_texCoord = {0.0f, 1.0f};
    }
    else
    {
        theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
                .m_texCoord = coords[3];
    }
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount].m_texIndex =
            tex_slot;
    theRenderStorage.m_vertices[theRenderStorage.m_quadVertexCount]
            .m_tilingFactor = tiling;
    theRenderStorage.m_quadVertexCount++;

    theRenderStorage.m_quadIndexCount += 6;

    theRenderStorage.m_stats.m_quadCount++;
}

void
Renderer2D::drawQuad(
        const glm::vec2 &position,
        const glm::vec2 &size,
        const glm::vec4 &color)
{
    drawQuad({position.x, position.y, 0.0f}, size, color);
}

void
Renderer2D::drawQuad(
        const glm::vec3 &position,
        const glm::vec2 &size,
        const glm::vec4 &color)
{
    const glm::mat4 transform = grCalcTransform(position, size);
    grSubmitRender(transform, color);
}

void
Renderer2D::drawQuad(
        const glm::vec2 &position,
        const glm::vec2 &size,
        std::shared_ptr<Texture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    drawQuad(
            {position.x, position.y, 0.0f}, size, std::move(tex), tiling, tint);
}

void
Renderer2D::drawQuad(
        const glm::vec3 &position,
        const glm::vec2 &size,
        std::shared_ptr<Texture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    const glm::mat4 transform = grCalcTransform(position, size);
    grSubmitRender(transform, tint, tex, tiling);
}

void
Renderer2D::drawQuad(
        const glm::vec2 &position,
        const glm::vec2 &size,
        std::shared_ptr<SubTexture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    drawQuad(
            {position.x, position.y, 0.0f}, size, std::move(tex), tiling, tint);
}
void
Renderer2D::drawQuad(
        const glm::vec3 &position,
        const glm::vec2 &size,
        std::shared_ptr<SubTexture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    const glm::mat4 transform = grCalcTransform(position, size);
    grSubmitRender(
            transform, tint, tex->texture(), tiling,
            tex->texCoords());
}
void
Renderer2D::drawQuad(const glm::mat4 &transform, const glm::vec4 &color)
{
    grSubmitRender(transform, color);
}
void
Renderer2D::drawQuad(
        const glm::mat4 &transform,
        std::shared_ptr<Texture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    grSubmitRender(transform, tint, tex, tiling);
}

void
Renderer2D::drawRotatedQuad(
        const glm::vec2 &position,
        const glm::vec2 &size,
        float rotation,
        const glm::vec4 &color)
{
    drawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
}
void
Renderer2D::drawRotatedQuad(
        const glm::vec3 &position,
        const glm::vec2 &size,
        float rotation,
        const glm::vec4 &color)
{
    const glm::mat4 transform = grCalcTransform(position, size, rotation);
    grSubmitRender(transform, color);
}

void
Renderer2D::drawRotatedQuad(
        const glm::vec2 &position,
        const glm::vec2 &size,
        float rotation,
        std::shared_ptr<Texture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    drawRotatedQuad(
            {position.x, position.y, 0.0f}, size, rotation, std::move(tex),
            tiling, tint);
}

void
Renderer2D::drawRotatedQuad(
        const glm::vec3 &position,
        const glm::vec2 &size,
        float rotation,
        std::shared_ptr<Texture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    const glm::mat4 transform = grCalcTransform(position, size, rotation);
    grSubmitRender(transform, tint, tex, tiling);
}

void
Renderer2D::drawRotatedQuad(
        const glm::vec2 &position,
        const glm::vec2 &size,
        float rotation,
        std::shared_ptr<SubTexture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    drawRotatedQuad(
            {position.x, position.y, 0.0f}, size, rotation, std::move(tex),
            tiling, tint);
}
void
Renderer2D::drawRotatedQuad(
        const glm::vec3 &position,
        const glm::vec2 &size,
        float rotation,
        std::shared_ptr<SubTexture2D> tex,
        float tiling,
        const glm::vec4 &tint)
{
    const glm::mat4 transform = grCalcTransform(position, size, rotation);
    grSubmitRender(
            transform, tint, tex->texture(), tiling,
            tex->texCoords());
}

void
Renderer2D::resetStats()
{
    memset(&theRenderStorage.m_stats, 0, sizeof(Statistics));
}
const Renderer2D::Statistics &
Renderer2D::statistics()
{
    return theRenderStorage.m_stats;
}

} // namespace dogb::GR