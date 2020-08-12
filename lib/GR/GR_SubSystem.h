//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_SUBSYSTEM_H
#define DOGBONE_SUBSYSTEM_H

#include "GR_API.h"

#include <UT/UT_Window.h>

namespace dogb::GR
{

class DB_GR_API SubSystem
{
public:
    static SubSystem& instance();

    void init() ;
    void shutdown()  ;
    void update(const UT::Timestep& ) ;

private:
    SubSystem() = default;
};

}

#endif // DOGBONE_SUBSYSTEM_H
