//
// Created by matt on 2020-08-04.
//

#include "GS_Mesh.h"

#include <GR/GR_Renderer2D.h>
#include <GR/GR_Renderer.h>

#include <UT/UT_Profiler.h>

namespace dogb::GS
{
void
Mesh::draw(const glm::mat4 &transform)
{
    UT_PROFILE_FUNCTION();

    // TODO: Ideally this will show a standard material on creation instead of
    //  just showing nothing.
    if (!m_material)
        return;

    GS::Material& _mat = *m_material;

    if (!_mat)
        return;

    _mat.m_shader->bind();

    _mat.uploadValues();
    _mat.bindTextures();

    // Set the entity ID.
    //_mat.m_shader->setInt(GR::RenderNames::u_EntityID, static_cast<int>(m_entity.handle()));

    GR::Renderer::submit(_mat.m_shader.get(), m_VAO, transform);

    _mat.unbindTextures();

    _mat.m_shader->unbind();
}

} // namespace dogb::GS
