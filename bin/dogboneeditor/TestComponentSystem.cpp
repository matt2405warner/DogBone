//
// Created by matt on 2020-07-28.
//

#include "TestComponentSystem.h"

#include "TestSpriteComponent.h"

#include <GS/GS_World.h>
#include <GS/GS_TransformComponent.h>

#include <GR/GR_Renderer2D.h>

DB_COMPONENT_SYS_REGISTRATION(dogb::TestComponentSystem)

namespace dogb
{
void
TestComponentSystem::onUpdate(const UT::Timestep &)
{
    GS::World& world = GS::World::instance();
    std::shared_ptr<GR::Camera> main_camera = world.mainCamera();

    if (!main_camera)
        return;

    GR::Renderer2D::beginScene(*main_camera);

    auto group =
            world.m_entityManager.registry().group<GS::TransformComponent>(
                    entt::get<dogb::TestSpriteComponent>);
    for (auto entity : group)
    {
        auto &&[transform, sprite] =
        group.get<GS::TransformComponent, TestSpriteComponent>(
                entity);
        GR::Renderer2D::drawQuad(transform, sprite);
    }

    GR::Renderer2D::endScene();
}
TestComponentSystem::TestComponentSystem() : GS::ComponentSystem()
{
    m_sysGroupType = GS::SystemGroupType::PRESENTATION;
}

}