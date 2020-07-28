//
// Created by matt on 2020-07-24.
//

#include "GS_EntityManager.h"

#include "GS_Entity.h"
#include "GS_TransformComponent.h"
#include "GS_TagComponent.h"
#include "GS_World.h"

#include <fmt/format.h>

namespace dogb::GS
{
rttr::type EntityManager::Info::theInvalid = rttr::type::get<void>();

EntityManager &
Entity::manager()
{
    return GS::World::instance().m_entityManager;
}

Entity
EntityManager::createEntity()
{
    Entity entity{m_registry.create()};
    // Every component must! have a transform component
    addComponent<TransformComponent>(entity);
    TagComponent& tag = addComponent<TagComponent>(entity);
    tag.m_name = fmt::format("Entity {}", entity.m_handle);
    return entity;
}

} // namespace dogb::GS