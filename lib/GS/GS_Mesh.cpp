//
// Created by matt on 2020-08-04.
//

#include "GS_Mesh.h"

#include <GR/GR_Renderer.h>

#include <UT/UT_Profiler.h>

namespace dogb::GS
{
void
Mesh::draw(const glm::mat4 &transform)
{
    UT_PROFILE_FUNCTION();

    m_shader->bind();
    m_texture->bind();

    GR::Renderer::submit(m_shader.get(), m_VAO, transform);

    m_texture->unbind();
    m_shader->unbind();
}

} // namespace dogb::GS
