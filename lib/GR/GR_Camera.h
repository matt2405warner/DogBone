//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_GR_CAMERA_H
#define DOGBONE_GR_CAMERA_H

#include "GR_API.h"

#include "GR_Framebuffer.h"

#include <rttr/type>

#include <memory>

namespace dogb::GR
{
class DB_GR_API Camera
{
    RTTR_ENABLE()
public:
    Camera();
    virtual ~Camera() = default;

    void calculateMatricies(glm::mat4& proj) const;

    bool m_orthographic = true;
    glm::vec4 m_rect;
    std::shared_ptr<Framebuffer> m_activeTexture;
};
}

#endif // DOGBONE_GR_CAMERA_H
