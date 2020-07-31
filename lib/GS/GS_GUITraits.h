//
// Created by matt on 2020-07-30.
//

#ifndef DOGBONE_GS_GUITRAITS_H
#define DOGBONE_GS_GUITRAITS_H

namespace dogb::GS
{
enum GUI_TRAITS : unsigned
{
    GUI_NONE = 1 << 0,
    GUI_HIDE = 1 << 1,
    GUI_NO_REMOVE = 1 << 2
};

template <typename T>
constexpr GUI_TRAITS GUITypeTraits()
{
    return GUI_NONE;
}

}

#endif // DOGBONE_GS_GUITRAITS_H
