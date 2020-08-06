//
// Created by matt on 2020-07-13.
//

#ifndef DOGBONE_GS_CAMERACONTROLLER_H
#define DOGBONE_GS_CAMERACONTROLLER_H

#include "GS_API.h"

#include <UT/UT_Timestep.h>

namespace dogb::GS
{

struct DB_GS_API CameraController
{
public:
    static constexpr const char* theGUIName = "Camera Controller";

    CameraController();

    void onGUI();

    float m_moveSpeed;
    float m_rotateSpeed;
    bool m_enableRotate;
};

} // namespace dogb::GS

#endif // DOGBONE_GS_CAMERACONTROLLER_H
