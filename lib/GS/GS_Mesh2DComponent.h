//
// Created by matt on 2020-07-23.
//

#ifndef DOGBONE_GS_MESH2DCOMPONENT_H
#define DOGBONE_GS_MESH2DCOMPONENT_H

#include "GS_API.h"

#include <GR/GR_Color.h>

#include <UT/UT_Logger.h>
#include <glm/glm.hpp>

namespace dogb::GS
{

struct DB_GS_API Mesh2DComponent
{
public:
    static constexpr const char* theGUIName = "Mesh";

    Mesh2DComponent() { UT_LOG_INFO("Create Mesh"); }
    Mesh2DComponent(const glm::vec4 &color) : m_color(color) { UT_LOG_INFO("Create mesh from value"); }
    virtual ~Mesh2DComponent() {UT_LOG_INFO("Destroy mesh");}

    void onGUI();

    GR::Color m_color{1.0f, 1.0f, 1.0f, 1.0f};
};

}

#endif // DOGBONE_GS_MESH2DCOMPONENT_H
