//
// Created by matt on 2020-07-24.
//

#ifndef DOGBONE_GS_ENTITYMANAGER_H
#define DOGBONE_GS_ENTITYMANAGER_H

#include "GS_API.h"

#include "GS_Editor.h"
#include "GS_GUITraits.h"
#include "GS_SystemGroup.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Logger.h>

#include <imgui.h>

#include <rttr/type>

#include <entt/entt.hpp>

#include <UT/UT_Timestep.h>
#include <array>
#include <functional>
#include <vector>

namespace dogb::GS
{
class Entity;
class EntityManager;

class DB_GS_API Entity
{
public:
    using IdType = entt::entity;

    Entity() = default;
    explicit Entity(entt::entity handle) : m_handle(handle) {}

    explicit operator bool() const { return m_handle != entt::null && m_manager; }
    explicit operator IdType() const { return m_handle; }

    bool operator<(const Entity &entity) const
    {
        return m_handle < entity.m_handle;
    }
    bool operator>(const Entity &entity) const
    {
        return m_handle > entity.m_handle;
    }
    bool operator>=(const Entity &entity) const
    {
        return (*this > entity) || (*this == entity);
    }
    bool operator<=(const Entity &entity) const
    {
        return (*this < entity) || (*this == entity);
    }
    bool operator==(const Entity &entity) const
    {
        return m_handle == entity.m_handle;
    }
    bool operator!=(const Entity &entity) const { return !(*this == entity); }

    template <typename T, typename... Args>
    T &addComponent(Args &&... args);
    template <typename T>
    T &getComponent();
    template <typename T>
    bool hasComponent();
    template <typename T>
    void removeComponent();

    void clear() { m_handle = entt::null; }

    void addChildEntity(const Entity &child);
    const Entity &parent();
    const Entity &root();
    const std::vector<Entity> &children();

private:
    std::optional<std::reference_wrapper<EntityManager>> m_manager;

    friend class EntityManager;

    IdType m_handle = entt::null;
};

namespace details
{
template <typename T>
void internalRemove(GS::EntityManager &mgr, GS::Entity &e);
template <typename T>
void internalGUI(GS::EntityManager &mgr, GS::Entity &e);
template <typename T>
void internalCreate(GS::EntityManager &mgr, GS::Entity &e);
} // namespace details

class DB_GS_API EntityManager
{
public:
    struct DB_GS_API Info
    {
        using Callback = std::function<void(GS::EntityManager &, GS::Entity &)>;

        std::string m_name;
        Callback m_createCallback;
        Callback m_removeCallback;
        Callback m_guiCallback;
        // Hold a garbage type
        rttr::type m_type = theInvalid;

    private:
        static rttr::type theInvalid;
    };

    Entity createEntity();

    using Registry = entt::registry;

    template <typename T, typename... Args>
    T &addComponent(Entity &entity, Args &&... args)
    {
        // Create the callbacks for this new component type.
        if (auto it = m_types.find(entt::type_info<T>::id());
            it == m_types.end())
        {
            Info info{
                    .m_name = T::theGUIName,
                    .m_createCallback = details::internalCreate<T>,
                    .m_removeCallback = details::internalRemove<T>,
                    // NB: We only need these two for editor related things. We
                    // should
                    //      ifdef this so that its not included in an actual
                    //      runtime.
                    .m_guiCallback = details::internalGUI<T>,
                    .m_type = rttr::type::get<T>(),
            };

            m_types.emplace(entt::type_info<T>::id(), info);
        }

        T &comp = m_registry.emplace<T>(
                static_cast<Entity::IdType>(entity),
                std::forward<Args>(args)...);

        return comp;
    }

    void addComponent(Entity &entity, entt::id_type id)
    {
        // Find our component type and add the component
        if (auto it = m_types.find(id); it == m_types.end())
        {
            it->second.m_createCallback(*this, entity);
        }
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
        std::array<entt::id_type, 1> arr{comp_type};
        return m_registry.runtime_view(arr.cbegin(), arr.cend())
                .contains(e.m_handle);
    }

    template <typename T>
    void removeComponent(Entity &e)
    {
        UT_ASSERT_MSG(
                isValid(e), "Cannot remove component from invalid entity");
        m_registry.remove<T>(static_cast<Entity::IdType>(e));
    }

    bool isValid(const Entity &e) const
    {
        return m_registry.valid(static_cast<Entity::IdType>(e));
    }

    void destroy(Entity &e)
    {
        entt::entity entity = static_cast<entt::entity>(e);
        m_registry.destroy(entity);
        e.m_handle = entt::null;
    }

    Registry &registry() { return m_registry; }

    std::unordered_map<entt::id_type, Info> m_types;

private:
    entt::registry m_registry;
};

namespace details
{
template <typename T, typename = void>
struct HasOnGUIMemFn : std::false_type
{
};

template <typename T>
struct HasOnGUIMemFn<
        T,
        std::enable_if_t<
                std::is_member_function_pointer_v<decltype(&T::onGUI)>>>
    : std::true_type
{
};

template <typename T, typename = void>
struct HasGUITraits : std::false_type
{
};

template <typename T>
struct HasGUITraits<T, decltype(std::declval<T>().m_guiTraits, void())>
    : std::true_type
{
};

template <typename T>
constexpr bool HasOnGUIMemFn_v = HasOnGUIMemFn<T>::value;
template <typename T>
constexpr bool HasGUITraits_v = HasGUITraits<T>::value;

template <typename T>
void
internalRemove(GS::EntityManager &mgr, GS::Entity &e)
{
    mgr.removeComponent<T>(e);
}

template <typename T>
void
internalGUI([[maybe_unused]] GS::EntityManager &mgr, GS::Entity &e)
{
    constexpr GUI_TRAITS traits = GUITypeTraits<T>();

    // Check if we want to hide the component
    if constexpr (traits & GUI_HIDE)
    {
        return;
    }
    else
    {
        ImGui::PushID(static_cast<int>(rttr::type::get<T>().get_id()));

        bool no_remove = traits & GUI_NO_REMOVE;

        // If we dont want to remove the component then grey it out.
        if (no_remove)
        {
            ImVec4 grey{0.2f, 0.2f, 0.2f, 1.0f};
            ImGui::PushStyleColor(ImGuiCol_Button, grey);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, grey);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, grey);
        }

        if (ImGui::Button("-") && !no_remove)
        {
            mgr.removeComponent<T>(e);
            ImGui::PopID();
            return;
        }
        else
        {
            ImGui::SameLine();
        }

        if (no_remove)
            ImGui::PopStyleColor(3);

        if (ImGui::CollapsingHeader(T::theGUIName))
        {
            ImGui::Indent(30.0f);

            ImGui::PushID("Widget");
            T &comp = e.getComponent<T>();

            if constexpr (HasOnGUIMemFn_v<T>)
            {
                comp.onGUI();
            }
            else
            {
                dogb::EditorGUI<T>(comp);
            }
            ImGui::PopID();

            ImGui::Unindent(30.0f);
        }
        ImGui::PopID();
    }
}

template <typename T>
void
internalCreate(GS::EntityManager &mgr, GS::Entity &e)
{
    mgr.addComponent<T>(e);
}

} // namespace details

template <typename T, typename... Args>
T &
Entity::addComponent(Args &&... args)
{
    UT_ASSERT(m_manager);
    return m_manager->get().addComponent<T>(*this, std::forward<Args>(args)...);
}
template <typename T>
T &
Entity::getComponent()
{
    UT_ASSERT(m_manager);
    return m_manager->get().getComponent<T>(*this);
}

template <typename T>
bool
Entity::hasComponent()
{
    UT_ASSERT(m_manager);
    if (!m_manager)
        return false;

    return m_manager->get().hasComponent<T>(*this);
}

template <typename T>
void
Entity::removeComponent()
{
    UT_ASSERT(m_manager);
    if (!m_manager)
        return;

    m_manager->get().removeComponent<T>(*this);
}

} // namespace dogb::GS

#endif // DOGBONE_GS_ENTITYMANAGER_H
