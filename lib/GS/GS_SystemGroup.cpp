//
// Created by matt on 2020-07-27.
//

#include "GS_SystemGroup.h"

#include "GS_World.h"
#include "GS_EntityManager.h"

#include <UT/UT_Assert.h>

namespace dogb::GS
{

SystemGroup::SystemGroup(SystemGroupType type) : m_type(type) {}

void
SystemGroup::addSystem(std::unique_ptr<ComponentSystem> sys)
{
    if (!sys)
    {
        UT_ASSERT_MSG(false, "Unable to add system without valid memory.");
        return;
    }

    rttr::type::type_id update_before = sys->m_updateBefore;

    if (update_before)
    {
        for (auto it = m_systems.begin(); it != m_systems.end(); it++)
        {
            if ((*it)->get_type().get_id() == update_before)
            {
                m_systems.insert(it, std::move(sys));
                break;
            }
        }
    }
    else
        m_systems.emplace_back(std::move(sys));
}

void
SystemGroup::onPreUpdate(const UT::Timestep & ts)
{
    World &world = World::instance();
    auto active_scene = world.m_activeScene;
    UT_ASSERT(active_scene);
    EntityManager &mgr = active_scene->m_entityManager;

    for (auto&& sys : m_systems)
        sys->onPreUpdate(ts, mgr);
}

void
SystemGroup::onUpdate(const UT::Timestep& ts)
{
    World &world = World::instance();
    auto active_scene = world.m_activeScene;
    UT_ASSERT(active_scene);
    EntityManager &mgr = active_scene->m_entityManager;

    for (auto&& sys : m_systems)
        sys->onUpdate(ts, mgr);
}

void
SystemGroup::onPostUpdate(const UT::Timestep &ts)
{
    World &world = World::instance();
    auto active_scene = world.m_activeScene;
    UT_ASSERT(active_scene);
    EntityManager &mgr = active_scene->m_entityManager;

    for (auto&& sys : m_systems)
        sys->onPostUpdate(ts, mgr);
}
void
SystemGroup::onShutdown()
{
    World &world = World::instance();
    auto active_scene = world.m_activeScene;
    UT_ASSERT(active_scene);
    EntityManager &mgr = active_scene->m_entityManager;

    for (auto&& sys : m_systems)
        sys->onShutdown(mgr);
}

} // namespace dogb::GS
