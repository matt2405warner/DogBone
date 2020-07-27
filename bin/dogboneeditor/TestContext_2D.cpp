//
// Created by matt on 2020-07-14.
//

#include "TestContext_2D.h"

#include "SceneWindow.h"

#include <GS/GS_Scene.h>
#include <GS/GS_SubSystem.h>
#include <GS/GS_TransformComponent.h>

#include <GR/GR_Renderer.h>
#include <GR/GR_Renderer2D.h>

#include <UT/UT_Assert.h>
#include <UT/UT_Profiler.h>

#include <imgui.h>

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    rttr::registration::class_<dogb::SpriteRendererComponent>("TestComponent")
            .constructor<>()
            .property("m_color", &dogb::SpriteRendererComponent::m_color)(
                    rttr::metadata("min_value", 0.0f),
                    rttr::metadata("max_value", 1.0f));
}

namespace dogb
{
TestContext_2D::TestContext_2D(SceneWindow *scene_window)
    : m_sceneWindow(scene_window)
{
}

void
TestContext_2D::onInit()
{
    UT_PROFILE_FUNCTION();

    m_texture = GR::Texture2D::create("../assets/textures/Checkerboard.png");

    UT::Engine &engine = UT::Engine::get();
    GS::SubSystem *gs_system = engine.getOrCreateSubSystem<GS::SubSystem>();

    auto scene = gs_system->m_activeScene;
    UT_ASSERT(scene != nullptr);

    m_testEntity = scene->createEntity();
    m_testEntity.addComponent<SpriteRendererComponent>(
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
    // m_testEntity.addComponent<TestComponent>(5);

    scene->m_selectedEntity = m_testEntity;
}

void
TestContext_2D::update(UT::Timestep ts)
{
    UT_PROFILE_FUNCTION();

    UT::Engine &engine = UT::Engine::get();
    GS::SubSystem *gs_system = engine.getOrCreateSubSystem<GS::SubSystem>();

    auto scene = gs_system->m_activeScene;

    GR::Renderer::setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    GR::Renderer::clear();

    m_framebuffer->bind();

    GR::Renderer::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    GR::Renderer::clear();

    GR::Renderer2D::beginScene(m_sceneWindow->m_cameraController.camera());

#if 1
    (void)ts;

    auto group =
            scene->m_entityManager.registry().group<GS::TransformComponent>(
                    entt::get<dogb::SpriteRendererComponent>);
    for (auto entity : group)
    {
        auto &&[transform, sprite] =
                group.get<GS::TransformComponent, SpriteRendererComponent>(
                        entity);
        GR::Renderer2D::drawQuad(transform, sprite);
    }

#else
    GR::Renderer2D::drawQuad(
            {-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
    GR::Renderer2D::drawQuad(
            {0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
    GR::Renderer2D::drawQuad({0.2f, 0.5f}, {0.5f, 0.75f}, m_texture, 10.0f);
    GR::Renderer2D::drawQuad({-0.2f, -0.5f}, {0.5f, 0.5f}, m_texture, 2.0f);
    GR::Renderer2D::drawRotatedQuad(
            {-0.8f, -0.8f}, {0.5f, 0.5f}, 45.0f, m_texture, 10.0f,
            {0.2f, 0.8f, 0.3f, 1.0f});
    static float rotation = 0.0f;
    rotation += ts.seconds() * 20.0f;
    GR::Renderer2D::drawRotatedQuad(
            {0.8f, -0.8f}, {0.5f, 0.5f}, rotation, m_texture, 10.0f,
            {0.2f, 0.8f, 0.3f, 1.0f});
    // GR::Renderer2D::endScene();

    // GR::Renderer2D::beginScene(m_cameraController.camera());

    for (float y = -5.0f; y < 5.0f; y += 0.5f)
    {
        for (float x = -5.0f; x < 5.0f; x += 0.5f)
        {
            glm::vec4 color = {(x + 5) / 10, 0.4f, (y + 5) / 10, 0.5f};
            GR::Renderer2D::drawQuad({x, y, 1.0f}, {0.45f, 0.45f}, color);
        }
    }
#endif

    GR::Renderer2D::endScene();

    m_framebuffer->unbind();
}
} // namespace dogb