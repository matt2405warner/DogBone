//
// Created by matt on 2020-07-29.
//

#include "GS_RenderComponentSystem.h"

#include <GR/GR_Renderer.h>
#include <GR/GR_Renderer2D.h>

#include "GS_CameraComponent.h"
#include "GS_MeshComponent.h"
#include "GS_TransformComponent.h"
#include "GS_World.h"

DB_COMPONENT_SYS_REGISTRATION(dogb::GS::RenderComponentSystem)

namespace dogb::GS
{
RenderComponentSystem::RenderComponentSystem()
{
    m_sysGroupType = GS::SystemGroupType::PRESENTATION;
}

void
RenderComponentSystem::onUpdate(const dogb::UT::Timestep &)
{
    World &world = World::instance();
    EntityManager &mgr = world.m_entityManager;

    auto camera_group =
            mgr.registry().view<CameraComponent, TransformComponent>();
    for (auto &camera_entity : camera_group)
    {
        auto &&[camera, cam_transform] =
                camera_group.get<CameraComponent, TransformComponent>(
                        camera_entity);

        if (!camera.m_camera || camera.m_camera->m_isDisabled)
            continue;

        if (camera.m_camera->m_activeTexture)
            camera.m_camera->m_activeTexture->bind();

        GR::Renderer::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        GR::Renderer::clear();

        GR::Renderer2D::beginScene(*camera.m_camera, glm::mat4(1.0f));

        auto mesh_group =
                mgr.registry().group<TransformComponent, MeshComponent>();
        for (auto &mesh_entity : mesh_group)
        {
            auto &&[mesh_transform, mesh] =
                    mesh_group.get<TransformComponent, MeshComponent>(
                            mesh_entity);
            GR::Renderer2D::drawQuad(mesh_transform, mesh.m_color.toVec4());
        }

        GR::Renderer2D::endScene();

        if (camera.m_camera->m_activeTexture)
            camera.m_camera->m_activeTexture->unbind();
    }
}
} // namespace dogb::GS