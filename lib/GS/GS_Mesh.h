//
// Created by matt on 2020-08-04.
//

#ifndef DOGBONE_GS_MESH_H
#define DOGBONE_GS_MESH_H

#include "GS_API.h"

#include <GR/GR_VertexArray.h>
#include <GR/GR_RenderBuffer.h>
#include <GR/GR_Shader.h>
#include <GR/GR_Texture.h>

#include <memory>

namespace dogb::GS
{
class DB_GS_API Mesh
{
public:
    std::shared_ptr<GR::VertexArray> m_VAO;
    // Eventually this will be a material
    std::shared_ptr<GR::Texture> m_texture;
    std::shared_ptr<GR::Shader> m_shader;

    void draw(const glm::mat4& transform);
};
}

#endif // DOGBONE_GS_MESH_H
