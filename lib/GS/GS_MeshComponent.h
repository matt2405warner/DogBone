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
    REFLECT_COMPONENT(MeshComponent, "Mesh")

    MeshComponent() = default;
    virtual ~MeshComponent() = default;

    void serialize(GS_YAML::Emitter& emitter) const;
    void deserialize(GS_YAML::Node& node);
    void onGUI();

    Mesh m_mesh;

private:
    void selectFileCallback_(const std::string& file);
};
}

#endif // DOGBONE_GS_MESHCOMPONENT_H
