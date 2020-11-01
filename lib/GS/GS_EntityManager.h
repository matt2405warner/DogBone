//
// Created by matt on 2020-07-24.
//

#ifndef DOGBONE_GS_ENTITYMANAGER_H
#define DOGBONE_GS_ENTITYMANAGER_H

#include "GS_API.h"

#include "GS_Editor.h"
#include "GS_GUITraits.h"
#include "GS_SystemGroup.h"
#include "GS_YAML.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Logger.h>
#include <UT/UT_Debug.h>

#include <imgui.h>
#include <imgui_internal.h>

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
    explicit Entity(entt::entity handle, EntityManager& mgr) :
        m_manager(mgr), m_handle(handle)
    {}

    explicit operator bool() const;
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
    const T& getComponent() const;
    template <typename T>
    bool hasComponent();
    template <typename T>
    void removeComponent();

    void clear() { m_handle = entt::null; }

    void addChildEntity(const Entity &child);
    Entity &parent();
    Entity &root();
    std::vector<Entity> &children();

private:
    std::optional<std::reference_wrapper<EntityManager>> m_manager;

    friend class EntityManager;

    IdType m_handle = entt::null;
};

namespace details
{
#define DEFINE_HAS_MEM_FUN(cls, F)                                             \
    template <typename T, typename = void>                                     \
    struct cls : std::false_type                                               \
    {                                                                          \
    };                                                                         \
                                                                               \
    template <typename T>                                                      \
    struct cls<                                                                \
            T,                                                                 \
            std::enable_if_t<std::is_member_function_pointer_v<decltype(F)>>>  \
        : std::true_type                                                       \
    {                                                                          \
    };
DEFINE_HAS_MEM_FUN(HasOnGUIMemFn, &T::onGUI)
DEFINE_HAS_MEM_FUN(HasSerializeMemFn, &T::serialize)
DEFINE_HAS_MEM_FUN(HasDeserializeMemFn, &T::deserialize)

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
constexpr bool HasSerializeMemFn_v = HasSerializeMemFn<T>::value;
template <typename T>
constexpr bool HasDeserializeMemFn_v = HasDeserializeMemFn<T>::value;
template <typename T>
constexpr bool HasGUITraits_v = HasGUITraits<T>::value;
}

template <typename T>
void DrawComponent(GS::EntityManager &mgr, GS::Entity &e);

template <typename T>
void SerializeComponent(YAML::Emitter& emitter, const GS::Entity& e)
{
    UTformatDebug("Serialize type: '{}'", T::theTypename);
    emitter << YAML::Key << T::theTypename;
    emitter << YAML::BeginMap; // Begin: Component
    if constexpr (details::HasSerializeMemFn_v<T>)
    {
        const T &comp = e.getComponent<T>();
        comp.serialize(emitter);
    }
    emitter << YAML::EndMap; // End: Component
}

template <typename T>
void DeserializeComponent(YAML::Node& node, GS::Entity& e)
{
    auto data = node[T::theTypename];
    if (!data)
        return;

    T& comp = e.addComponent<T>();
    if constexpr (details::HasDeserializeMemFn_v<T>)
    {
        comp.deserialize(data);
    }
}

namespace details
{
template <typename T>
void internalRemove(GS::EntityManager &mgr, GS::Entity &e);
template <typename T>
void internalCreate(GS::EntityManager &mgr, GS::Entity &e);
} // namespace details

class DB_GS_API EntityManager
{
public:
    Entity createEntity(const std::string& name = std::string());

    using Registry = entt::registry;

    template <typename T, typename... Args>
    T &addComponent(Entity &entity, Args &&... args)
    {
        T &comp = m_registry.emplace<T>(
                static_cast<Entity::IdType>(entity),
                std::forward<Args>(args)...);

        return comp;
    }

    void addComponent(Entity &entity, entt::id_type id);

    template <typename T>
    T &getComponent(const Entity &entity)
    {
        return m_registry.get<T>(static_cast<Entity::IdType>(entity));
    }

    template <typename T>
    bool hasComponent(const Entity &e) const
    {
        return m_registry.has<T>(static_cast<Entity::IdType>(e));
    }

    bool hasComponent(const Entity &e, const entt::id_type &comp_type) const
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

    void destroy(Entity &e);

    Registry &registry() { return m_registry; }

private:
    entt::registry m_registry;
};

namespace details
{

template <typename T>
void
internalRemove(GS::EntityManager &mgr, GS::Entity &e)
{
    mgr.removeComponent<T>(e);
}

template <typename T>
void
internalCreate(GS::EntityManager &mgr, GS::Entity &e)
{
    mgr.addComponent<T>(e);
}

} // namespace details

template <typename T>
void
DrawComponent([[maybe_unused]] GS::EntityManager &mgr, GS::Entity &e)
{
    constexpr GUI_TRAITS traits = GUITypeTraits<T>();

    // Check if we want to hide the component
    if constexpr (traits & GUI_HIDE)
    {
        return;
    }
    else
    {
        ImVec2 region = ImGui::GetContentRegionAvail();
        const bool no_remove = traits & GUI_NO_REMOVE;

        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
        const float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        const bool open = ImGui::TreeNodeEx(T::theGUIName, flags, T::theGUIName);
        ImGui::PopStyleVar();

        bool remove_component = false;

        if (!no_remove)
        {
            ImGui::SameLine(region.x - line_height * 0.5f);

            if (ImGui::Button("-", {line_height, line_height}))
            {
                remove_component = true;
            }
        }

        if (open)
        {
            T &comp = e.getComponent<T>();

            if constexpr (details::HasOnGUIMemFn_v<T>)
            {
                comp.onGUI();
            }
            else
            {
                dogb::EditorGUI<T>(comp);
            }

            ImGui::TreePop();
        }

        if (remove_component)
            e.removeComponent<T>();
    }
}

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
const T &
Entity::getComponent() const
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

inline
Entity::operator bool() const
{
    return m_handle != entt::null && m_manager && m_manager->get().isValid(*this);
}

} // namespace dogb::GS

#endif // DOGBONE_GS_ENTITYMANAGER_H
