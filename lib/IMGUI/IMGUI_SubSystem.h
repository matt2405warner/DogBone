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
class DB_IMGUI_API SubSystem : public UT::SubSystem
{
public:
    using context_t = IMGUI::SubSystemContext;

    void init() override;
    void shutdown() override;

    void update(const UT::Timestep &timestep) override;

    void attach(std::shared_ptr<context_t> ctx)
    {
        ctx->onAttach();
        m_contexts.emplace_back(ctx);
    }

    template <typename T>
    T* getWindow() const
    {
        static_assert(
                std::is_base_of_v<IMGUI::Window, T>,
                "Cannot get GUI window if it does not derive from IMGUI::Window");

        for (auto&& ctx : m_contexts)
        {
            T* window = ctx->getGUIWindow<T>();
            if (window != nullptr)
                return window;
        }
        return nullptr;
    }

    Modal* addModal(std::unique_ptr<Modal> modal);
private:
    context_t * m_currentCtx;
    std::vector<std::shared_ptr<context_t>> m_contexts;
};
} // namespace dogb::IMGUI

#endif // DOGBONE_IMGUI_SUBSYSTEM_H
