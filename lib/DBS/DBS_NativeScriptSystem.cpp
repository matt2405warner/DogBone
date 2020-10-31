//
// Created by matt on 2020-10-31.
//

#include "DBS_NativeScriptSystem.h"

DB_COMPONENT_SYS_REGISTRATION(dogb::DBS::NativeScriptSystem)

#include "DBS_NativeScriptComponent.h"

namespace dogb::DBS
{
NativeScriptSystem::NativeScriptSystem()
{
    m_sysGroupType = GS::SystemGroupType::SIMULATION;
}

void
NativeScriptSystem::onUpdate(
        const UT::Timestep &timestep,
        const GS::SceneSPtr& scene)
{
    GS::EntityManager& mgr = scene->m_entityManager;
    mgr.registry().view<NativeScriptComponent>().each([timestep, &mgr](auto entity, auto& nsc)
    {
        if (!nsc.isInitialised())
        {
            nsc.initialise(GS::Entity{.m_handle=entity, .m_manager=mgr});
        }

        nsc.update(timestep);

        // TODO: add a way to remove the script when its finished.
    });
}
}