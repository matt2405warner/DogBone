//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_GS_SUBSYSTEM_H
#define DOGBONE_GS_SUBSYSTEM_H

#include "GS_API.h"

#include "GS_SubSystemContext.h"

#include <UT/UT_Timestep.h>
#include <UT/UT_SubSystem.h>

#include <vector>
#include <memory>

namespace dogb::GS
{
class SubSystem : public UT::SubSystem
{
public:
    using context_t = GS::SubSystemContext;

    void init() override {}
    void shutdown() override;

    void update(const UT::Timestep &timestep) override;

    void attach(std::shared_ptr<context_t> ctx) { ctx->onAttach(); m_contexts.emplace_back(ctx); }
private:
    std::vector<std::shared_ptr<context_t>> m_contexts;
};
}

#endif // DOGBONE_GS_SUBSYSTEM_H
