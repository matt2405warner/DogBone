//
// Created by matt on 2020-07-26.
//

#ifndef DOGBONE_GS_EDITOR_H
#define DOGBONE_GS_EDITOR_H

#include "GS_API.h"

#include <rttr/type>

#include <type_traits>

namespace dogb
{
template <typename T>
void EditorGUI(T &item);

namespace GS
{
class DB_GS_API EditorBase
{
    template <typename T>
    friend void dogb::EditorGUI(T &);

protected:
    using Callback = std::function<void(rttr::instance&, const rttr::type& t, const rttr::property&)>;

    template <typename T>
    void internalOnGUI(T &item)
    {
        rttr::type t = rttr::type::get<T>();

        if (!t.is_valid())
            return;

        if (t.is_class())
        {
            for (auto &prop : t.get_properties())
            {
                if (!prop.is_valid())
                    continue;

                // Only grab the public access properties
                if (prop.get_access_level() !=
                    rttr::access_levels::public_access)
                    continue;

                if (prop.is_static())
                    continue;

                rttr::type prop_t = prop.get_type();

                if (auto it = m_typeCallbacks.find(prop_t.get_id());
                    it != m_typeCallbacks.end())
                {
                    if (it->second)
                    {
                        rttr::instance _instance(item);
                        (*it).second(_instance, t, prop);
                    }
                }
            }
        }
        else if (t.is_array())
        {
        }
        else if (t.is_enumeration())
        {
        }
    }

    std::unordered_map<rttr::type::type_id, Callback> m_typeCallbacks;

    // The implementation of the action editor.
    static std::unique_ptr<EditorBase> theImpl;
};
} // namespace GS

template <typename T>
void
EditorGUI(T &item)
{
    GS::EditorBase::theImpl->internalOnGUI<T>(item);
}

} // namespace dogb

#endif // DOGBONE_GS_EDITOR_H
