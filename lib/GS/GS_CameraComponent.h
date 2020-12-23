//
// Created by matt on 2020-07-29.
//

#ifndef DOGBONE_GS_CAMERACOMPONENT_H
#define DOGBONE_GS_CAMERACOMPONENT_H

#include "GS_API.h"

#include "GS_Camera.h"
#include "GS_YAML.h"
#include "GS_ComponentTypeRegistry.h"

#include <rttr/type>

namespace dogb::GS
{
class DB_GS_API CameraComponent
{
public:
    REFLECT_COMPONENT(CameraComponent, "Camera")

    CameraComponent();
    virtual ~CameraComponent() = default;

    void serialize(GS_YAML::Emitter& emitter) const;
    void deserialize(GS_YAML::Node& node);
    void onGUI();

    [[nodiscard]] bool isPrimary() const
    {
        if (!m_camera)
            return false;
        return m_camera->m_primary;
    }

    std::shared_ptr<GS::Camera> m_camera;
};
}

#endif // DOGBONE_GS_CAMERACOMPONENT_H
