//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GS_SCENE_H
#define DOGBONE_GS_SCENE_H

#include "GS_API.h"

#include <UT/UT_Timestep.h>

#include <entt/entt.hpp>

namespace dogb::GS
{

class Scene
{
public:
    void update(const UT::Timestep& ts);
    entt::entity createEntity();

    entt::registry & registry() { return m_registry; }

private:
    entt::registry m_registry;
};

}

#endif // DOGBONE_GS_SCENE_H
