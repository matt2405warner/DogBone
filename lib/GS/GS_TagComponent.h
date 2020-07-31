//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_GS_TAGCOMPONENT_H
#define DOGBONE_GS_TAGCOMPONENT_H

#include "GS_API.h"

#include "GS_GUITraits.h"

#include <rttr/type>

#include <string>

namespace dogb::GS
{

struct DB_GS_API TagComponent
{
public:
    static constexpr const char* theGUIName = "Tag";

    virtual ~TagComponent() = default;

    GUI_TRAITS m_guiTraits = GUI_HIDE;

    std::string m_name;
};

template <>
constexpr GUI_TRAITS GUITypeTraits<TagComponent>()
{
    return GUI_HIDE;
}
}

#endif // DOGBONE_GS_TAGCOMPONENT_H
