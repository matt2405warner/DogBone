//
// Created by matt on 2020-07-28.
//

#include "GS_TagComponent.h"

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace rttr;
    rttr::registration::class_<dogb::GS::TagComponent>("TagComponent")
            (rttr::metadata("GUI_HIDE", true))
            .constructor<>()
            .property("m_name", &dogb::GS::TagComponent::m_name);
}