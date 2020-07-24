//
// Created by matt on 2020-07-23.
//

#include "GS_Scene.h"

#include "GS_Entity.h"
#include "GS_TransformComponent.h"

namespace dogb::GS
{
void
Scene::update(const UT::Timestep &)
{
}

Entity
Scene::createEntity()
{
    Entity entity{m_registry.create(), this};
    // Every component must! have a transform component
    entity.addComponent<TransformComponent>();
    return entity;
}

} // namespace dogb::GS
