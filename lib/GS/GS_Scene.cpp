//
// Created by matt on 2020-07-23.
//

#include "GS_Scene.h"
#include "GS_CameraComponent.h"

namespace dogb::GS
{

void
Scene::onViewportResize(uint32_t width, uint32_t height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;

    // Resize non-fixed aspect ratio cameras
    auto view = m_entityManager.registry().view<CameraComponent>();

    for (auto entity : view)
    {
        auto& cam_comp = view.get<CameraComponent>(entity);
        if (cam_comp.m_camera->m_fixedAspectRatio)
            continue;

        cam_comp.m_camera->setViewportSize(width, height);
    }
}
Entity
Scene::createEntity(const std::string& name)
{
    return m_entityManager.createEntity(name);
}

} // namespace dogb::GS
