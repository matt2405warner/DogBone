//
// Created by matt on 2020-11-01.
//

#include "GS_ComponentTypeRegistry.h"

namespace dogb::GS
{
rttr::type ComponentTypeRegistry::Info::theInvalid = rttr::type::get<void>();

ComponentTypeRegistry &
ComponentTypeRegistry::instance()
{
    static ComponentTypeRegistry _instance;
    return _instance;
}

}