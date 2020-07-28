//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_GS_TAGCOMPONENT_H
#define DOGBONE_GS_TAGCOMPONENT_H

#include "GS_API.h"

#include <rttr/type>

#include <string>

namespace dogb::GS
{
struct DB_GS_API TagComponent
{
    RTTR_ENABLE()
public:
    virtual ~TagComponent() = default;
    std::string m_name;
};
}

#endif // DOGBONE_GS_TAGCOMPONENT_H
