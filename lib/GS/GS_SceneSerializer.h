//
// Created by matt on 2020-10-31.
//

#ifndef DOGBONE_GS_SCENESERIALIZER_H
#define DOGBONE_GS_SCENESERIALIZER_H

#include "GS_API.h"

#include "GS_Scene.h"

#include <string>

namespace dogb::GS
{
class DB_GS_API SceneSerializer
{
public:
    explicit SceneSerializer(const SceneSPtr& scene);

    void serialize(const std::string& filepath);

    bool deserialize(const std::string& filepath);
private:
    SceneSPtr m_scene;
};
}

#endif // DOGBONE_GS_SCENESERIALIZER_H
