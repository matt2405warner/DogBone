//
// Created by matt on 2020-07-27.
//

#ifndef DOGBONE_GS_SYSTEMGROUP_H
#define DOGBONE_GS_SYSTEMGROUP_H

#include "GS_API.h"

#include "GS_ComponentSystem.h"
#include "GS_SystemGroupDefines.h"

#include <UT/UT_Timestep.h>

#include <memory>
#include <vector>

#include <rttr/type>

namespace dogb::GS
{
class DB_GS_API SystemGroup
{
    RTTR_ENABLE()

public:
    explicit SystemGroup(SystemGroupType type);
    virtual ~SystemGroup() = default;
    void addSystem(std::unique_ptr<ComponentSystem> sys);
    virtual void onPreUpdate(const UT::Timestep&);
    virtual void onUpdate(const UT::Timestep& ts);
    virtual void onPostUpdate(const UT::Timestep&);
    virtual void onShutdown();
    [[nodiscard]] SystemGroupType type() const { return m_type; }
private:
    SystemGroupType m_type;
    std::vector<std::unique_ptr<ComponentSystem>> m_systems;
};

} // namespace dogb::GS

#endif // DOGBONE_GS_SYSTEMGROUP_H
