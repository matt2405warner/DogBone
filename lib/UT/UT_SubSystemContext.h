//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_UT_SUBSYSTEMCONTEXT_H
#define DOGBONE_UT_SUBSYSTEMCONTEXT_H

#include "UT_API.h"

#include "UT_Timestep.h"

#include <unordered_map>
#include <utility>
#include <memory>

#include <rttr/type>

namespace dogb::UT
{
class DB_UT_API SubSystemContext
{
public:
    virtual ~SubSystemContext() = default;

    virtual void update(UT::Timestep ts) = 0;
};

} // namespace dogb::UT

#endif // DOGBONE_UT_SUBSYSTEMCONTEXT_H
