//
// Created by matt on 2020-07-24.
//

#include "GS_Entity.h"

namespace dogb::GS
{
Entity::Entity(entt::entity handle, Scene* scene)
    : m_handle(handle), m_scene(scene)
{
}

} // namespace dogb::GS