//
// Created by matt on 2020-07-23.
//

#include "GS_MeshComponent.h"

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    rttr::registration::class_<dogb::GS::MeshComponent>("Mesh")
            .constructor<>()
            .property("m_color", &dogb::GS::MeshComponent::m_color)
                    (
                            rttr::metadata("min_value", 0.0f),
                            rttr::metadata("max_value", 1.0f));
}