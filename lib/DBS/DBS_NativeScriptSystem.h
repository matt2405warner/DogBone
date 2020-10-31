//
// Created by matt on 2020-10-31.
//

#ifndef DOGBONE_DBS_NATIVESCRIPTSYSTEM_H
#define DOGBONE_DBS_NATIVESCRIPTSYSTEM_H

#include "DBS_API.h"
#include "DBS_NativeScriptComponent.h"

#include <GS/GS_ComponentSystem.h>
#include <GS/GS_EntityManager.h>
#include <GS/GS_Scene.h>

#include <UT/UT_Timestep.h>

namespace dogb::DBS
{
class DB_DBS_API NativeScriptSystem : public GS::ComponentSystem
{
DB_COMPONENT_SYS_ENABLE()
public:
    NativeScriptSystem();
    void onUpdate(const UT::Timestep &ts, const GS::SceneSPtr& scene) override;
};

} // namespace dogb::GS

#endif // DOGBONE_DBS_NATIVESCRIPTSYSTEM_H
