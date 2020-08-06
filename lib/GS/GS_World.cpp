//
// Created by matt on 2020-07-27.
//

#include "GS_World.h"
#include "GS_RenderSystemGroup.h"

#include <GR/GR_OrthoGraphicCamera.h>

namespace dogb::GS
{

World &
World::instance()
{
    static World _world;
    return _world;
}
void
World::update(const UT::Timestep &ts)
{
    m_systemInitGroup->onPreUpdate(ts);
    m_systemSimGroup->onPreUpdate(ts);
    m_systemPresGroup->onPreUpdate(ts);

    m_systemInitGroup->onUpdate(ts);
    m_systemSimGroup->onUpdate(ts);
    m_systemPresGroup->onUpdate(ts);

    m_systemInitGroup->onPostUpdate(ts);
    m_systemSimGroup->onPostUpdate(ts);
    m_systemPresGroup->onPostUpdate(ts);
}
void
World::init()
{
    m_activeScene = std::make_shared<GS::Scene>();
    m_mainCamera = std::make_shared<GS::Camera>();

    m_systemInitGroup = std::make_shared<SystemGroup>(SystemGroupType::INIT);
    m_systemSimGroup = std::make_shared<SystemGroup>(SystemGroupType::SIMULATION);
    m_systemPresGroup = std::make_shared<RenderSystemGroup>();

    rttr::type component_system = rttr::type::get<ComponentSystem>();

    rttr::array_range<rttr::type> children = component_system.get_derived_classes();
    for (auto&& child : children)
    {
        rttr::variant obj_var = child.create();
        auto sys = std::unique_ptr<ComponentSystem>(obj_var.convert<ComponentSystem*>());
        if (!sys)
            continue;

        if (sys->systemGroup() == SystemGroupType::INIT)
            m_systemInitGroup->addSystem(std::move(sys));
        else if (sys->systemGroup() == SystemGroupType::PRESENTATION)
            m_systemPresGroup->addSystem(std::move(sys));
        else if (sys->systemGroup() == SystemGroupType::SIMULATION)
            m_systemSimGroup->addSystem(std::move(sys));
    }
}

void
World::shutdown()
{
    m_systemInitGroup->onShutdown();
    m_systemSimGroup->onShutdown();
    m_systemPresGroup->onShutdown();

    // I cant seem to find any way to clear all entities so for now we just
    // reset the entire manager on shutdown.
    m_activeScene = nullptr;

    m_mainCamera = nullptr;
}

Entity
World::createEntity()
{
    return m_activeScene->createEntity();
}
std::shared_ptr<SystemGroup>
World::getSystemGroup(SystemGroupType type)
{
    if (type == SystemGroupType::SIMULATION)
        return m_systemSimGroup;
    else if (type == SystemGroupType::PRESENTATION)
        return m_systemPresGroup;
    else if (type == SystemGroupType::INIT)
        return m_systemInitGroup;
    return nullptr;
}

}
