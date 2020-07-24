//
// Created by matt on 2020-07-14.
//

#include "GR_SubSystem.h"

#include "GR_Renderer.h"
#include "GR_Renderer2D.h"

namespace dogb::GR
{
void
SubSystem::update(const UT::Timestep &)
{
    Renderer2D::resetStats();
}
void
SubSystem::init()
{
    GR::Renderer::init();
    GR::Renderer2D::init();
}
void
SubSystem::shutdown()
{
    GR::Renderer2D::shutdown();
}

}
