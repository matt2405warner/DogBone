//
// Created by matt on 2020-07-28.
//

#include "TestSpriteComponent.h"

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    rttr::registration::class_<dogb::TestSpriteComponent>("TestComponent")
            .constructor<>()
            .property("m_color", &dogb::TestSpriteComponent::m_color)(
                    rttr::metadata("min_value", 0.0f),
                    rttr::metadata("max_value", 1.0f));
}