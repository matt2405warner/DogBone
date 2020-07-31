//
// Created by matt on 2020-07-29.
//

#ifndef DOGBONE_GS_CAMERA_H
#define DOGBONE_GS_CAMERA_H

#include "GS_API.h"

#include <GR/GR_Camera.h>

#include <CE/CE_Asset.h>

#include <rttr/type.h>

namespace dogb::GS
{
class DB_GS_API Camera : public GR::Camera, public CE::Asset
{
    RTTR_ENABLE(GR::Camera, CE::Asset)
public:
    void onGUI();

    bool m_isDisabled = false;
};
}

#endif // DOGBONE_GS_CAMERA_H
