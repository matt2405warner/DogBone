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

    void setViewportSize(uint32_t width, uint32_t height);

    bool m_orthographic = true;
    float m_near;
    float m_far;
    float m_orthographicSize;
    float m_aspect;
    std::shared_ptr<Framebuffer> m_activeTexture;
};
}

#endif // DOGBONE_GR_CAMERA_H
