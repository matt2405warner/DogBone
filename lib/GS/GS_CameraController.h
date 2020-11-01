//
// Created by matt on 2020-07-13.
//

#ifndef DOGBONE_GS_CAMERACONTROLLER_H
#define DOGBONE_GS_CAMERACONTROLLER_H

#include "GS_API.h"

#include "GS_YAML.h"
#include "GS_ComponentTypeRegistry.h"

#include <UT/UT_Timestep.h>

namespace dogb::GS
{

struct DB_GS_API CameraController
{
public:
    REFLECT_COMPONENT(CameraController, "Camera Controller")

    CameraController();

    void onGUI();
    void serialize(GS_YAML::Emitter& emitter) const;
    void deserialize(GS_YAML::Node& node);

    float m_moveSpeed;
    float m_rotateSpeed;
    bool m_enableRotate;
};

} // namespace dogb::GS

#endif // DOGBONE_GS_CAMERACONTROLLER_H
