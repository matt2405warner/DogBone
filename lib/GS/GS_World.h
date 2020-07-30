//
// Created by matt on 2020-07-27.
//

#ifndef DOGBONE_GS_WORLD_H
#define DOGBONE_GS_WORLD_H

#include "GS_API.h"

#include "GS_SystemGroupDefines.h"
#include "GS_SystemGroup.h"
#include "GS_Entity.h"
#include "GS_Camera.h"

namespace dogb::GS
{

class DB_GS_API World
{
public:
    static World& instance();

    void init();
    void shutdown();

    void update(const UT::Timestep& ts);
    Entity createEntity();

    EntityManager m_entityManager;
    Entity m_selectedEntity;

    std::shared_ptr<SystemGroup> getSystemGroup(SystemGroupType type);

    std::shared_ptr<GS::Camera> mainCamera() { return m_mainCamera; }
private:
    std::shared_ptr<GS::Camera> m_mainCamera;

    std::shared_ptr<SystemGroup> m_systemInitGroup;
    std::shared_ptr<SystemGroup> m_systemSimGroup;
    std::shared_ptr<SystemGroup> m_systemPresGroup;
};

}

#endif // DOGBONE_GS_WORLD_H
