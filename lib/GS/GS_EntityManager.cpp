//
// Created by matt on 2020-07-24.
//

#include "GS_EntityManager.h"

#include "GS_TagComponent.h"
#include "GS_TransformComponent.h"
#include "GS_World.h"

#include <fmt/format.h>

namespace dogb::GS
{
rttr::type EntityManager::Info::theInvalid = rttr::type::get<void>();

void
Entity::addChildEntity(const Entity &child)
{
    UT_ASSERT(m_manager);
    if (!m_manager)
        return;

    TransformComponent &transform =
            m_manager->get().getComponent<TransformComponent>(*this);

    for (auto &&e : transform.m_children)
    {
        if (e == child)
            return;
    }

    TransformComponent &child_transform =
            m_manager->get().getComponent<TransformComponent>(child);
    child_transform.m_parent = *this;
    child_transform.m_root = transform.m_root;

    transform.m_children.emplace_back(child);
}

Entity &
Entity::parent()
{
    UT_ASSERT(m_manager);
    TransformComponent &transformComponent =
            m_manager->get().getComponent<TransformComponent>(*this);
    return transformComponent.m_parent;
}

Entity &
Entity::root()
{
    UT_ASSERT(m_manager);
    TransformComponent &transformComponent =
            m_manager->get().getComponent<TransformComponent>(*this);
    return transformComponent.m_root;
}

std::vector<Entity> &
Entity::children()
{
    UT_ASSERT(m_manager);
    TransformComponent &transformComponent =
            m_manager->get().getComponent<TransformComponent>(*this);
    return transformComponent.m_children;
}

Entity
EntityManager::createEntity()
{
    Entity entity{m_registry.create()};
    entity.m_manager = *this;

    // Every component must! have a transform component
    TransformComponent &transform = addComponent<TransformComponent>(entity);
    // The root is our self but the parent is no one. This allows us to traverse
    // the entities tree.
    transform.m_root = entity;

    TagComponent &tag = addComponent<TagComponent>(entity);
    tag.m_name = fmt::format("Entity {}", entity.m_handle);
    return entity;
}

void
EntityManager::destroy(Entity &e)
{
    entt::entity entity = static_cast<entt::entity>(e);

    // Remove the entity from the parent
    TransformComponent &root = e.parent().getComponent<TransformComponent>();
    root.removeChild(e);

    m_registry.destroy(entity);
    e.m_handle = entt::null;
}

} // namespace dogb::GS