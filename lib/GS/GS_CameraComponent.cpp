//
// Created by matt on 2020-07-29.
//

#include "GS_CameraComponent.h"

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    rttr::registration::class_<dogb::GS::CameraComponent>("CameraComponent")
            .constructor<>()
            .property("m_camera", &dogb::GS::CameraComponent::m_camera);
}