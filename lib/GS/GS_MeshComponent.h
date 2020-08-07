//
// Created by matt on 2020-08-04.
//

#ifndef DOGBONE_GS_MESHCOMPONENT_H
#define DOGBONE_GS_MESHCOMPONENT_H

#include "GS_API.h"

#include "GS_Mesh.h"

namespace dogb::GS
{
struct DB_GS_API MeshComponent
{
public:
    static constexpr const char* theGUIName = "Mesh";

    MeshComponent() = default;
    virtual ~MeshComponent() = default;

    void onGUI();

    Mesh m_mesh;
};
}

#endif // DOGBONE_GS_MESHCOMPONENT_H
