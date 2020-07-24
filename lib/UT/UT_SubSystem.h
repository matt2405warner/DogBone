//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_UT_SUBSYSTEM_H
#define DOGBONE_UT_SUBSYSTEM_H

#include "UT_API.h"

#include "UT_Timestep.h"

#include <vector>
#include <memory>

namespace dogb::UT
{
class DB_UT_API SubSystem
{
public:
    virtual ~SubSystem() = default;
    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void update(const UT::Timestep& ts) = 0;
};
} // namespace dogb::UT

#endif // DOGBONE_UT_SUBSYSTEM_H
