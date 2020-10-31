//
// Created by matt on 2020-10-31.
//

#ifndef DOGBONE_DBS_NATIVESCRIPTCOMPONENT_H
#define DOGBONE_DBS_NATIVESCRIPTCOMPONENT_H

#include "DBS_API.h"

#include "DBS_ScriptableEntity.h"

namespace dogb::DBS
{

class DB_DBS_API NativeScriptComponent
{
    friend class NativeScriptSystem;
public:
    template <typename T>
    void bind()
    {
        m_instantiateScript = []() { return std::make_unique<T>(); };
    }

private:
    [[nodiscard]] bool isInitialised() const { return m_scriptableEntity != nullptr; }
    void initialise(const GS::Entity& e);
    void update(const UT::Timestep& ts);

    std::unique_ptr<ScriptableEntity>(*m_instantiateScript)();
    std::unique_ptr<ScriptableEntity> m_scriptableEntity;
};

}

#endif // DOGBONE_DBS_NATIVESCRIPTCOMPONENT_H
