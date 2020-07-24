//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_GS_SUBSYSTEMCONTEXT_H
#define DOGBONE_GS_SUBSYSTEMCONTEXT_H

#include "GS_API.h"

#include <UT/UT_SubSystemContext.h>

namespace dogb::GS
{
// TODO: add in the game object ECS update system here.
class SubSystemContext : public UT::SubSystemContext
{
public:
    virtual void onInit() {}
    virtual void onAttach() {}
    virtual void onShutdown() {}
};
}

#endif // DOGBONE_GS_SUBSYSTEMCONTEXT_H
