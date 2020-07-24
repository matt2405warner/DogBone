//
// Created by matt on 2020-07-24.
//

#include "GS_Entity.h"

namespace dogb::GS
{
Entity::Entity(entt::entity handle, EntityManager* mgr)
    : m_handle(handle), m_mgr(mgr)
{
}

} // namespace dogb::GS