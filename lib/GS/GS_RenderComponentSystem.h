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
    void onPreUpdate(const UT::Timestep& ts, const std::shared_ptr<Scene>& scene) override;
    void onUpdate(const UT::Timestep &ts, const std::shared_ptr<Scene>& scene) override;
};
}

#endif // DOGBONE_GS_RENDERCOMPONENTSYSTEM_H
