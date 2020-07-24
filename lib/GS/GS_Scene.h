//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GS_SCENE_H
#define DOGBONE_GS_SCENE_H

#include "GS_API.h"

#include "GS_EntityManager.h"

#include <UT/UT_Timestep.h>

#include <entt/entt.hpp>

namespace dogb::GS
{
class Entity;

class Scene
{
public:
    void update(const UT::Timestep& ts);

    Entity createEntity();

    EntityManager m_entityManager;
    Entity m_selectedEntity;
};

}

#endif // DOGBONE_GS_SCENE_H
