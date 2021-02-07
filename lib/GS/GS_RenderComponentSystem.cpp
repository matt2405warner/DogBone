//
// Created by matt on 2020-07-29.
//

#include "GS_RenderComponentSystem.h"

#include <GR/GR_Renderer.h>
#include <GR/GR_Renderer2D.h>

#include "GS_CameraComponent.h"
#include "GS_Editor.h"
#include "GS_Mesh2DComponent.h"
#include "GS_MeshComponent.h"
#include "GS_Scene.h"
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
RenderComponentSystem::onUpdate(
        const dogb::UT::Timestep &,
        const SceneSPtr &scene)
{
    EntityManager &mgr = scene->m_entityManager;

    // If its being run within the editor then render the editor camera in
    // addition to the other camera if any.
    if (Editor::isEditor())
    {
        auto frame_buffer = GS::Editor::instance().framebuffer();
        frame_buffer->bind();

        GR::Renderer::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        GR::Renderer::clear();

        // Clear our entity ID attachment to -1
        frame_buffer->clearAttachment(1, -1);

        // ---------------------------------------------------------------------
        // 2D Editor View
        // ---------------------------------------------------------------------
        GR::Renderer2D::beginScene(Editor::camera().viewProjection());
        auto mesh2d_view =
                mgr.registry().view<TransformComponent, Mesh2DComponent>();
        for (auto &mesh_entity : mesh2d_view)
        {
            auto &&[mesh_transform, mesh] =
            mesh2d_view.get<TransformComponent, Mesh2DComponent>(
                    mesh_entity);

            GR::Renderer2D::pushEntity((uint32_t)mesh_entity);
            GR::Renderer2D::drawQuad(
                    mesh_transform.transform(), mesh.m_color.toVec4());
        }
        GR::Renderer2D::endScene();
        // ---------------------------------------------------------------------
        // 3D Editor View
        // ---------------------------------------------------------------------
        GR::Renderer::beginScene(Editor::camera().viewProjection());
        auto mesh_view =
                mgr.registry().view<TransformComponent, MeshComponent>();
        for (auto &mesh_entity : mesh_view)
        {
            auto &&[mesh_transform, mesh] =
            mesh_view.get<TransformComponent, MeshComponent>(
                    mesh_entity);
            mesh.m_mesh.draw(mesh_transform.transform());
        }
        GR::Renderer::endScene();

        frame_buffer->unbind();
    }

    auto camera_group =
            mgr.registry().view<CameraComponent, TransformComponent>();

    for (auto &camera_entity : camera_group)
    {
        auto &&[camera, cam_t] =
                camera_group.get<CameraComponent, TransformComponent>(
                        camera_entity);

        if (!camera.m_camera || camera.m_camera->m_isDisabled)
            continue;

        if (camera.m_camera->m_activeTexture)
            camera.m_camera->m_activeTexture->bind();

        GR::Renderer::setClearColor(camera.m_camera->m_background);
        GR::Renderer::clear();

        glm::mat4 cam_transform = cam_t.transform();
        // ---------------------------------------------------------------------
        // BEGIN 2D Meshes
        // ---------------------------------------------------------------------
        GR::Renderer2D::beginScene(*camera.m_camera, cam_transform);
        auto mesh2d_view =
                mgr.registry().view<TransformComponent, Mesh2DComponent>();
        for (auto &mesh_entity : mesh2d_view)
        {
            auto &&[mesh_transform, mesh] =
                    mesh2d_view.get<TransformComponent, Mesh2DComponent>(
                            mesh_entity);
            GR::Renderer2D::drawQuad(
                    mesh_transform.transform(), mesh.m_color.toVec4());
        }
        GR::Renderer2D::endScene();
        // ---------------------------------------------------------------------
        // END 2D Meshes
        // ---------------------------------------------------------------------

        // ---------------------------------------------------------------------
        // BEGIN 3D Meshes
        // ---------------------------------------------------------------------
        GR::Renderer::beginScene(*camera.m_camera, cam_transform);
        auto mesh_view =
                mgr.registry().view<TransformComponent, MeshComponent>();
        for (auto &mesh_entity : mesh_view)
        {
            auto &&[mesh_transform, mesh] =
                    mesh_view.get<TransformComponent, MeshComponent>(
                            mesh_entity);
            mesh.m_mesh.draw(mesh_transform.transform());
        }
        GR::Renderer::endScene();
        // ---------------------------------------------------------------------
        // END 3D Meshes
        // ---------------------------------------------------------------------

        if (camera.m_camera->m_activeTexture)
            camera.m_camera->m_activeTexture->unbind();
    }
}
} // namespace dogb::GS