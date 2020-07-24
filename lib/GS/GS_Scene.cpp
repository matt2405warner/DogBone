//
// Created by matt on 2020-07-23.
//

#include "GS_Scene.h"

namespace dogb::GS
{

void
Scene::update(const UT::Timestep &)
{

}
entt::entity
Scene::createEntity()
{
    return m_registry.create();
}

}
