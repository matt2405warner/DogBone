//
// Created by matt on 2020-08-05.
//

#ifndef DOGBONE_GS_CAMERACONTROLLERSYSTEM_H
#define DOGBONE_GS_CAMERACONTROLLERSYSTEM_H

#include "GS_API.h"

#include "GS_ComponentSystem.h"

namespace dogb::GS
{
class DB_GS_API CameraControllerSystem : public ComponentSystem
{
    DB_COMPONENT_SYS_ENABLE()
public:
    CameraControllerSystem();
    void onUpdate(const UT::Timestep &ts, const std::shared_ptr<Scene>& scene) override;
};
} // namespace dogb::GS

#endif // DOGBONE_GS_CAMERACONTROLLERSYSTEM_H
