//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GS_SCENE_H
#define DOGBONE_GS_SCENE_H

#include "GS_API.h"

#include "GS_EntityManager.h"
#include "GS_ComponentSystem.h"

#include <UT/UT_Timestep.h>

#include <entt/entt.hpp>

#include <memory>

namespace dogb::GS
{
class Entity;

class Scene
{
public:
    void onViewportResize(uint32_t width, uint32_t height);

    Entity createEntity();

    EntityManager m_entityManager;

    uint32_t m_viewportWidth = 0;
    uint32_t m_viewportHeight = 0;
};

using SceneSPtr = std::shared_ptr<Scene>;

}

#endif // DOGBONE_GS_SCENE_H
