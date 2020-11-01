//
// Created by matt on 2020-07-27.
//

#ifndef DOGBONE_GS_COMPONENTSYSTEM_H
#define DOGBONE_GS_COMPONENTSYSTEM_H

#include "GS_API.h"

#include "GS_SystemGroupDefines.h"

#include <UT/UT_Timestep.h>

#include <rttr/registration>
#include <rttr/type>

#include <memory>

#define DB_COMPONENT_SYS_REGISTRATION(_cls_)                                   \
    RTTR_REGISTRATION                                                          \
    {                                                                          \
        using namespace rttr;                                                  \
        registration::class_<_cls_>(#_cls_).constructor<>()(                   \
                policy::ctor::as_raw_ptr);                                     \
    }

#define DB_COMPONENT_SYS_ENABLE() RTTR_ENABLE(dogb::GS::ComponentSystem)

namespace dogb::GS
{
class EntityManager;
class Scene;

class DB_GS_API ComponentSystem
{
    RTTR_ENABLE()

    friend class SystemGroup;
public:
    virtual ~ComponentSystem() = default;
#if 0
    virtual void onCreate() {};
    virtual void onDestroy() {};
#endif
    virtual void onPreUpdate(const UT::Timestep &, const std::shared_ptr<Scene>& ) {}
    virtual void onUpdate(const UT::Timestep &, const std::shared_ptr<Scene>& ) {}
    virtual void onPostUpdate(const UT::Timestep &, const std::shared_ptr<Scene>& ) {}
    virtual void onShutdown(const std::shared_ptr<Scene>& ) {}

    [[nodiscard]] SystemGroupType systemGroup() const { return m_sysGroupType; }

protected:
    ComponentSystem();

    rttr::type::type_id m_updateBefore;
    SystemGroupType m_sysGroupType = SystemGroupType::SIMULATION;
};
} // namespace dogb::GS

#endif // DOGBONE_GS_COMPONENTSYSTEM_H
