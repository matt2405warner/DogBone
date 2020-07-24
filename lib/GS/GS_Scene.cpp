//
// Created by matt on 2020-07-23.
//

#include "GS_Scene.h"

#include "GS_EntityManager.h"
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
    return m_entityManager.createEntity();
}

} // namespace dogb::GS
