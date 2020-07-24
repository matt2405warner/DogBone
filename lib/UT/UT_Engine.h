//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_UT_ENGINE_H
#define DOGBONE_UT_ENGINE_H

#include "UT_API.h"

#include "UT_Logger.h"
#include "UT_SubSystem.h"

#include <memory>
#include <unordered_map>

#include <rttr/type>

namespace dogb::UT
{
class UT_API Engine
{
public:
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    static Engine &get();

    template <typename T>
    T *getOrCreateSubSystem()
    {
        static_assert(
                std::is_base_of_v<SubSystem, T>, "Get subsystem requires"
                                                 " type that derives from 'SubSystem'");
        return getOrCreateSubSystemImpl<T>();
    }

private:
    Engine() = default;

    template <typename T>
    T *getOrCreateSubSystemImpl()
    {
        rttr::type t = rttr::type::get<T>();
        if (!t.is_valid())
        {
            UT_LOG_ERROR("Unable to get runtime type information");
            return nullptr;
        }
        auto it = m_systems.find(t.get_id());
        if (it != m_systems.end())
            return reinterpret_cast<T *>(it->second.get());

        UT_LOG_DEBUG("Creating subsystem");
        auto created_iter = m_systems.emplace(t.get_id(), std::make_unique<T>());
        if (!created_iter.second)
        {
            UT_LOG_ERROR("Failed to create subsystem");
            return nullptr;
        }

        return reinterpret_cast<T *>(created_iter.first->second.get());
    }

    std::unordered_map<rttr::type::type_id, std::unique_ptr<SubSystem>> m_systems;
};

} // namespace dogb::UT

#endif // DOGBONE_UT_ENGINE_H
