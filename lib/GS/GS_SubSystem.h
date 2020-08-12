//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_GS_SUBSYSTEM_H
#define DOGBONE_GS_SUBSYSTEM_H

#include "GS_API.h"

#include "GS_Scene.h"

#include <UT/UT_Timestep.h>
#include <UT/UT_SubSystem.h>

#include <vector>
#include <memory>

namespace dogb::GS
{
class SubSystem
{
public:
    static SubSystem& instance();
    void init();
    void shutdown();

    void update(const UT::Timestep &timestep);

    std::shared_ptr<Scene> m_activeScene;
private:
    SubSystem() =default;
};
}

#endif // DOGBONE_GS_SUBSYSTEM_H
