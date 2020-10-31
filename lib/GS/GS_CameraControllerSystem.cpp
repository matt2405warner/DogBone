//
// Created by matt on 2020-08-05.
//

#include "GS_CameraControllerSystem.h"

#include "GS_CameraComponent.h"
#include "GS_CameraController.h"
#include "GS_TransformComponent.h"
#include "GS_Scene.h"

#include <CE/CE_Input.h>

DB_COMPONENT_SYS_REGISTRATION(dogb::GS::CameraControllerSystem)

namespace dogb::GS
{
CameraControllerSystem::CameraControllerSystem()
{
    m_sysGroupType = GS::SystemGroupType::SIMULATION;
}

void
CameraControllerSystem::onUpdate(
        const dogb::UT::Timestep &timestep,
        const SceneSPtr& scene)
{
    EntityManager& mgr = scene->m_entityManager;
    auto view = mgr.registry()
                        .view<CameraComponent, TransformComponent,
                              CameraController>();
    for (auto &entity : view)
    {
        auto &&[camera, transform, controller] =
                view.get<CameraComponent, TransformComponent, CameraController>(
                        entity);

        float ts = timestep.seconds();

        using namespace CE::Input;

        float move_speed = controller.m_moveSpeed;

        glm::vec3 rotation = glm::degrees(transform.m_rotation);

        if (IsKeyPressed(KeyType::KEY_A))
        {
            transform.m_translation.x -= glm::cos(glm::radians(rotation.x)) *
                                      move_speed * ts;
            transform.m_translation.y -= glm::sin(glm::radians(rotation.y)) *
                                      move_speed * ts;
        }
        else if (IsKeyPressed(KeyType::KEY_D))
        {
            transform.m_translation.x += glm::cos(glm::radians(rotation.x)) *
                                      move_speed * ts;
            transform.m_translation.y += glm::sin(glm::radians(rotation.y)) *
                                      move_speed * ts;
        }

        if (IsKeyPressed(KeyType::KEY_W))
        {
            transform.m_translation.x += -glm::sin(glm::radians(rotation.x)) *
                                      move_speed * ts;
            transform.m_translation.y += glm::cos(glm::radians(rotation.y)) *
                                      move_speed * ts;
        }
        else if (IsKeyPressed(KeyType::KEY_S))
        {
            transform.m_translation.x -= -glm::sin(glm::radians(rotation.x)) *
                                      move_speed * ts;
            transform.m_translation.y -= glm::cos(glm::radians(rotation.y)) *
                                      move_speed * ts;
        }

        // TODO: add back in rotation
    }
}
} // namespace dogb::GS