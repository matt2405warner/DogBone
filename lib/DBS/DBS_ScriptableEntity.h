//
// Created by matt on 2020-10-31.
//

#ifndef DOGBONE_DBS_SCRIPTABLEENTITY_H
#define DOGBONE_DBS_SCRIPTABLEENTITY_H

#include "DBS_API.h"

#include <GS/GS_Entity.h>

#include <UT/UT_Timestep.h>

namespace dogb::DBS
{
class DB_DBS_API ScriptableEntity
{
    friend class NativeScriptComponent;
public:
    virtual ~ScriptableEntity() = default;

    template <typename T>
    T& getComponent()
    {
        UT_ASSERT(m_entity);
        return m_entity.getComponent<T>();
    }

protected:
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(const UT::Timestep&) {}

private:
    GS::Entity m_entity;
};
}

#endif // DOGBONE_DBS_SCRIPTABLEENTITY_H
