//
// Created by matt on 2020-07-23.
//

#include "GS_TransformComponent.h"

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace rttr;
    rttr::registration::class_<dogb::GS::TransformComponent>("TransformComponent")
            .constructor<>()
            .property("m_transform", &dogb::GS::TransformComponent::m_transform);
}