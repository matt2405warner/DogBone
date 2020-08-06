//
// Created by matt on 2020-07-29.
//

#ifndef DOGBONE_GS_RENDERCOMPONENTSYSTEM_H
#define DOGBONE_GS_RENDERCOMPONENTSYSTEM_H

#include "GS_API.h"

#include "GS_ComponentSystem.h"

namespace dogb::GS
{
class DB_GS_API RenderComponentSystem : public ComponentSystem
{
    DB_COMPONENT_SYS_ENABLE()
public:
    RenderComponentSystem();
    void onUpdate(const UT::Timestep &ts, EntityManager& mgr) override;
};
}

#endif // DOGBONE_GS_RENDERCOMPONENTSYSTEM_H
