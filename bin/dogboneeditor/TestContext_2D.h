//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_TESTCONTEXT_2D_H
#define DOGBONE_TESTCONTEXT_2D_H

#include <GS/GS_Entity.h>
#include <GS/GS_SubSystemContext.h>

#include <GR/GR_Framebuffer.h>
#include <GR/GR_Texture.h>

#include <UT/UT_SubSystemContext.h>
#include <UT/UT_Timestep.h>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

#include <memory>

namespace dogb
{
struct SpriteRendererComponent
{
    RTTR_ENABLE()
public:
    SpriteRendererComponent() = default;
    SpriteRendererComponent(const glm::vec4 &color) : m_color(color) {}
    virtual ~SpriteRendererComponent() = default;

    operator glm::vec4 &() { return m_color; }
    operator const glm::vec4 &() const { return m_color; }

    glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
};

class SceneWindow;

class TestContext_2D : public GS::SubSystemContext
{
public:
    explicit TestContext_2D(SceneWindow *scene_window);

    void onInit() override;

    void update(UT::Timestep ts) override;

    std::shared_ptr<GR::Texture2D> m_texture;
    std::shared_ptr<GR::Framebuffer> m_framebuffer;

    SceneWindow *m_sceneWindow;

    GS::Entity m_testEntity;
};

} // namespace dogb

#endif // DOGBONE_TESTCONTEXT_2D_H
