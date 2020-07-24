//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_SUBSYSTEM_H
#define DOGBONE_SUBSYSTEM_H

#include "GR_API.h"

#include <UT/UT_SubSystem.h>
#include <UT/UT_SubSystemContext.h>
#include <UT/UT_Window.h>

namespace dogb::GR
{

class DB_GR_API SubSystem : public UT::SubSystem
{
public:
    void init() override;
    void shutdown() override ;
    void update(const UT::Timestep& ) override;
};

}

#endif // DOGBONE_SUBSYSTEM_H
