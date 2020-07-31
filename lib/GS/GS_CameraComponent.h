//
// Created by matt on 2020-07-29.
//

#ifndef DOGBONE_GS_CAMERACOMPONENT_H
#define DOGBONE_GS_CAMERACOMPONENT_H

#include "GS_API.h"

#include "GS_Camera.h"

#include <rttr/type>

namespace dogb::GS
{
class DB_GS_API CameraComponent
{
public:
    static constexpr const char* theGUIName = "Camera";

    virtual ~CameraComponent()
    {
        // Disable the camera now that we are not using it.
        if (m_camera)
            m_camera->m_isDisabled = true;
    }

    void onGUI();

    std::shared_ptr<GS::Camera> m_camera;
};
}

#endif // DOGBONE_GS_CAMERACOMPONENT_H
