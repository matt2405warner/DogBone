//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_IMGUI_SUBSYSTEM_H
#define DOGBONE_IMGUI_SUBSYSTEM_H

#include "IMGUI_API.h"

#include "IMGUI_SubSystemContext.h"

#include <UT/UT_Engine.h>
#include <UT/UT_SubSystem.h>
#include <UT/UT_Timestep.h>

#include <rttr/type.h>

#include <memory>
#include <string>
#include <vector>

namespace dogb::IMGUI
{
class IMGUI_API SubSystem : public UT::SubSystem
{
public:
    using context_t = IMGUI::SubSystemContext;

    void init() override {}
    void shutdown() override;

    void update(const UT::Timestep &timestep) override;

    void attach(std::shared_ptr<context_t> ctx)
    {
        ctx->onAttach();
        m_contexts.emplace_back(ctx);
    }

private:
    std::vector<std::shared_ptr<context_t>> m_contexts;
};
} // namespace dogb::IMGUI

#endif // DOGBONE_IMGUI_SUBSYSTEM_H
