//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_TESTCOMPONENTSYSTEM_H
#define DOGBONE_TESTCOMPONENTSYSTEM_H

#include <GS/GS_ComponentSystem.h>

namespace dogb
{
class TestComponentSystem : public GS::ComponentSystem
{
    DB_COMPONENT_SYS_ENABLE()
public:
    TestComponentSystem();
    void onUpdate(const UT::Timestep& ts) override ;
};
}

#endif // DOGBONE_TESTCOMPONENTSYSTEM_H
