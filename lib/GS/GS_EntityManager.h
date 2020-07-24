//
// Created by matt on 2020-07-24.
//

#ifndef DOGBONE_GS_ENTITYMANAGER_H
#define DOGBONE_GS_ENTITYMANAGER_H

#include "GS_API.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Logger.h>

#include <rttr/type>

#include <entt/entt.hpp>

#include <functional>
#include <array>

namespace dogb::GS
{
class Entity;
class EntityManager;

class DB_GS_API Entity
{
public:
    using IdType = entt::entity;

    Entity() = default;
    Entity(entt::entity handle, EntityManager *scene);

    explicit operator bool() const { return m_handle == entt::null; }
    explicit operator IdType() const { return m_handle; }

    template <typename T, typename... Args>
    T &addComponent(Args &&... args);
    template <typename T>
    T &getComponent();
    template <typename T>
    bool hasComponent();
    template <typename T>
    void removeComponent();

private:
    friend class EntityManager;

    IdType m_handle = entt::null;

    EntityManager *m_mgr = nullptr;
};

namespace details
{
template <typename T>
void internalRemove(GS::EntityManager& mgr, GS::Entity& e);
template <typename T>
void internalGUI(GS::EntityManager& mgr, GS::Entity& e);
}

class DB_GS_API EntityManager
{
public:
    struct DB_GS_API Info
    {
        using Callback = std::function<void(GS::EntityManager&, GS::Entity&)>;

        std::string m_name;
        Callback m_removeCallback;
        Callback m_guiCallback;
    };

    Entity createEntity();

    using Registry = entt::registry ;

    template <typename T, typename... Args>
    T &addComponent(Entity &entity, Args &&... args)
    {
        // Create the callbacks for this new component type.
        if (auto it = m_types.find(entt::type_info<T>::id()); it == m_types.end())
        {
            Info info;
            info.m_name = rttr::type::get<T>().get_name().data();
            info.m_removeCallback = details::internalRemove<T>;
            info.m_guiCallback = details::internalGUI<T>;
            m_types.emplace(entt::type_info<T>::id(), info);
        }

        T &comp = m_registry.emplace<T>(
                static_cast<Entity::IdType>(entity),
                std::forward<Args>(args)...);

        return comp;
    }

    template <typename T>
    T &getComponent(const Entity &entity)
    {
        return m_registry.get<T>(static_cast<Entity::IdType>(entity));
    }

    template <typename T>
    bool hasComponent(const Entity &e)
    {
        return m_registry.has<T>(static_cast<Entity::IdType>(e));
    }

    bool hasComponent(Entity &e, const entt::id_type &comp_type)
    {
        std::array<entt::id_type , 1> arr{comp_type};
        return m_registry.runtime_view(arr.cbegin(), arr.cend())
                .contains(e.m_handle);
    }

    template <typename T>
    void removeComponent(Entity &e)
    {
        m_registry.remove<T>(static_cast<Entity::IdType>(e));
    }

    bool isValid(const Entity &e) const
    {
        return m_registry.valid(static_cast<Entity::IdType>(e));
    }

    Registry& registry() { return m_registry; }

    std::unordered_map<entt::id_type , Info> m_types;
private:

    entt::registry m_registry;
};

namespace details
{
template <typename T>
class has_onGUI
{
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static one test( typeof(&C::onGUI) ) ;
    template <typename C> static two test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

template <typename T>
void internalRemove(GS::EntityManager& mgr, GS::Entity& e)
{
    mgr.removeComponent<T>(e);
}

template <typename T>
void internalGUI(GS::EntityManager& mgr, GS::Entity& e)
{
    if constexpr (has_onGUI<T>::value)
    {
        T& comp = e.getComponent<T>();
        comp.onGUI(mgr, e);
    }
    else
    {
        UT_LOG_WARN("No GUI function found...");
        // TODO: figure out what to do here.
    }
}
}


template <typename T, typename... Args>
T &
Entity::addComponent(Args &&... args)
{
    UT_ASSERT(m_mgr != nullptr);
    return m_mgr->addComponent<T>(*this, std::forward<Args>(args)...);
}
template <typename T>
T &
Entity::getComponent()
{
    UT_ASSERT(m_mgr != nullptr);
    return m_mgr->getComponent<T>(*this);
}

template <typename T>
bool
Entity::hasComponent()
{
    UT_ASSERT(m_mgr != nullptr);
    return m_mgr->hasComponent<T>(*this);
}

template <typename T>
void
Entity::removeComponent()
{
    UT_ASSERT(m_mgr != nullptr);
    m_mgr->removeComponent<T>(*this);
}

} // namespace dogb::GS

#endif // DOGBONE_GS_ENTITYMANAGER_H
