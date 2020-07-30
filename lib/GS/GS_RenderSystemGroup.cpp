//
// Created by matt on 2020-07-28.
//

#include "GS_RenderSystemGroup.h"

#include "GS_World.h"

#include <GR/GR_Camera.h>
#include <GR/GR_Renderer.h>

namespace dogb::GS
{
RenderSystemGroup::RenderSystemGroup()
    : GS::SystemGroup(SystemGroupType::PRESENTATION)
{
}

void
RenderSystemGroup::onUpdate(const dogb::UT::Timestep &ts)
{
    GR::Renderer::setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    GR::Renderer::clear();

    SystemGroup::onUpdate(ts);
}
} // namespace dogb::GS
