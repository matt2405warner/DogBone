//
// Created by matt on 2020-11-01.
//

#ifndef DOGBONE_GS_COMPONENTTYPEREGISTRY_H
#define DOGBONE_GS_COMPONENTTYPEREGISTRY_H

#include "GS_API.h"

#include "GS_YAML.h"
#include "GS_EntityManager.h"

#include <rttr/type>

#include <entt/entt.hpp>

#include <functional>

namespace dogb::GS
{

class DB_GS_API ComponentTypeRegistry
{
public:
    struct Info
    {
        using Callback = std::function<void(GS::EntityManager &, GS::Entity &)>;
        using SerializeCallback = std::function<void(YAML::Emitter&, const GS::Entity&)>;
        using DeserializeCallback = std::function<void(YAML::Node&, GS::Entity&)>;

        std::string m_name;
        Callback m_createCallback;
        Callback m_removeCallback;
        Callback m_guiCallback;
        SerializeCallback m_serializeCallback;
        DeserializeCallback m_deserializeCallback;
        // Hold a garbage type
        rttr::type m_type = theInvalid;

    private:
        static rttr::type theInvalid;
    };

    static ComponentTypeRegistry& instance();

    template <typename T>
    static void registerComponentType()
    {
        // Create the callbacks for this new component type.
        ComponentTypeRegistry& registry = instance();
        if (auto it = registry.m_types.find(entt::type_info<T>::id());
                it == registry.m_types.end())
        {
            Info info{
                    .m_name = T::theGUIName,
                    .m_createCallback = details::internalCreate<T>,
                    .m_removeCallback = details::internalRemove<T>,
                    // NB: We only need these two for editor related things. We
                    // should
                    //      ifdef this so that its not included in an actual
                    //      runtime.
                    .m_guiCallback = DrawComponent<T>,
                    .m_serializeCallback = SerializeComponent<T>,
                    .m_deserializeCallback = DeserializeComponent<T>,
                    .m_type = rttr::type::get<T>(),
            };

            registry.m_types.emplace(entt::type_info<T>::id(), info);
        }
    }

    template <typename Func>
    static void each(Func&& func)
    {
        ComponentTypeRegistry& registry = instance();
        for (auto&&[id, info] : registry.m_types)
        {
            func(id, info);
        }
    }

    static void addComponent(EntityManager& mgr, Entity& e, const entt::id_type& id)
    {
        ComponentTypeRegistry& registry = instance();
        if (auto it = registry.m_types.find(id);
                it != registry.m_types.end())
        {
            it->second.m_createCallback(mgr, e);
        }
    }
private:

    std::unordered_map<entt::id_type, Info> m_types;
};

template <typename T>
class AutoComponentTypeRegister
{
public:
    AutoComponentTypeRegister()
    {
        ComponentTypeRegistry::registerComponentType<T>();
    }
};

}

#define CONCAT2(x, y) x ## y

#define AUTOREGISTER_TYPE(_cls_)                                               \
namespace dogb::GS::AutoTypeRegister                                           \
{                                                                              \
    static AutoComponentTypeRegister<_cls_> CONCAT2(__, _cls_);                \
}

#define REFLECT_COMPONENT(_cls_, _gui_) \
    constexpr static const char* theTypename = #_cls_; \
    constexpr static const char* theGUIName = _gui_;

#endif // DOGBONE_GS_COMPONENTTYPEREGISTRY_H
