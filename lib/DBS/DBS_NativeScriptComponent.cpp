//
// Created by matt on 2020-10-31.
//

#include "DBS_NativeScriptComponent.h"

namespace dogb::DBS
{

void
NativeScriptComponent::initialise(const GS::Entity &e)
{
    m_scriptableEntity = m_instantiateScript();
    m_scriptableEntity->m_entity = e;
    m_scriptableEntity->onCreate();
}
void
NativeScriptComponent::update(const UT::Timestep &ts)
{
    UT_ASSERT(m_scriptableEntity);
    m_scriptableEntity->onUpdate(ts);
}

}
