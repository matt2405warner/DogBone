//
// Created by matt on 2020-08-04.
//

#ifndef DOGBONE_GS_MESH_H
#define DOGBONE_GS_MESH_H

#include "GS_API.h"

#include "GS_Material.h"
#include "GS_YAML.h"
#include "GS_ComponentTypeRegistry.h"

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
    GS::MaterialPtr m_material;

    void draw(const glm::mat4& transform);
};
}

#endif // DOGBONE_GS_MESH_H
