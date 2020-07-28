//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_GS_RENDERSYSTEMGROUP_H
#define DOGBONE_GS_RENDERSYSTEMGROUP_H

#include "GS_API.h"

#include "GS_SystemGroup.h"

namespace dogb::GS
{
class DB_GS_API RenderSystemGroup : public GS::SystemGroup
{
public:
    RenderSystemGroup();
    void onUpdate(const UT::Timestep& ts) override ;
};
}

#endif // DOGBONE_GS_RENDERSYSTEMGROUP_H
