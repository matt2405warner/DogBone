//
// Created by matt on 2020-07-24.
//

#ifndef DOGBONE_GS_ENTITY_H
#define DOGBONE_GS_ENTITY_H

#include "GS_API.h"

#include "GS_Scene.h"
#include "GS_TransformComponent.h"

#include <UT/UT_Assert.h>

#include <entt/entt.hpp>

#include <memory>

namespace dogb::GS
{
class GS_API Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, Scene *scene);

    template <typename T, typename... Args>
    T &addComponent(Args &&... args)
    {
        UT_ASSERT_MSG(!hasComponent<T>(), "Entity already has component");
        UT_ASSERT(m_scene != nullptr);
        return m_scene->registry().emplace_or_replace<T>(
                m_handle, std::forward<Args>(args)...);
    }

    template <typename T>
    T &getComponent()
    {
        UT_ASSERT_MSG(hasComponent<T>(), "Entity does not have component");
        UT_ASSERT(m_scene != nullptr);
        return m_scene->registry().get<T>(m_handle);
    }

    template <typename T>
    bool hasComponent()
    {
        UT_ASSERT(m_scene != nullptr);
        return m_scene->registry().has<T>(m_handle);
    }

    template <typename T>
    void removeComponent()
    {
        // You cant remove a transform component.
        static_assert(std::is_same_v<TransformComponent, T>);
        UT_ASSERT_MSG(hasComponent<T>(), "Entity does not have component");
        UT_ASSERT(m_scene != nullptr);
        return m_scene->registry().remove<T>(m_handle);
    }

    explicit operator bool() const { return m_handle == entt::null; }

private:
    entt::entity m_handle = entt::null;
    Scene *m_scene = nullptr;
};

} // namespace dogb::GS

#endif // DOGBONE_GS_ENTITY_H
