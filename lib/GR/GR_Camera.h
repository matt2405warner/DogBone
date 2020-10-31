//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_GR_CAMERA_H
#define DOGBONE_GR_CAMERA_H

#include "GR_API.h"

#include "GR_Framebuffer.h"

#include <rttr/type>

#include <UT/UT_Assert.h>
#include <memory>

namespace dogb::GR
{
class DB_GR_API Camera
{
    RTTR_ENABLE()
public:
    Camera() = default;
    explicit Camera(const glm::mat4& projection) :
        m_projection(projection)
    {}
    virtual ~Camera() = default;

    [[nodiscard]] const glm::mat4& projection() const { return m_projection; }
protected:
    glm::mat4 m_projection;
};
}

#endif // DOGBONE_GR_CAMERA_H
