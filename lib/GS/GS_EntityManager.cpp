//
// Created by matt on 2020-07-24.
//

#include "GS_EntityManager.h"

#include "GS_Entity.h"
#include "GS_TransformComponent.h"

namespace dogb::GS
{

Entity
EntityManager::createEntity()
{
    Entity entity{m_registry.create(), this};
    // Every component must! have a transform component
    addComponent<TransformComponent>(entity);
    return entity;
}

} // namespace dogb::GS