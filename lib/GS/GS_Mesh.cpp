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

    GS::Material& _mat = *m_material;
    _mat.m_shader->bind();

    _mat.uploadValues();
    _mat.bindTextures();

    GR::Renderer::submit(_mat.m_shader.get(), m_VAO, transform);

    _mat.unbindTextures();

    _mat.m_shader->unbind();
}

} // namespace dogb::GS
