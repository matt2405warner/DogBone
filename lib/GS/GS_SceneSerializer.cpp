//
// Created by matt on 2020-10-31.
//

#include "GS_SceneSerializer.h"

#include "GS_EntityManager.h"
#include "GS_TagComponent.h"
#include "GS_TransformComponent.h"
#include "GS_ComponentTypeRegistry.h"

#include "GS_YAML.h"

#include <UT/UT_Debug.h>

#include <fstream>
#include <experimental/source_location>

namespace dogb::GS
{
SceneSerializer::SceneSerializer(const SceneSPtr &scene) :
    m_scene(scene)
{
}

static void
gsSerializeEntity(GS_YAML::Emitter& emitter, const Entity& e, const EntityManager& mgr)
{
    emitter << GS_YAML::BeginMap; // Begin: Entity

    // TODO:: Fix me as this needs to be a GUID value
    emitter << GS_YAML::Key << "Entity" << GS_YAML::Value << "1123";

    // Serialize the components of the entity
    GS::ComponentTypeRegistry::each([&](auto id, const auto& info){
        // Check if we can add this component.
        bool is_selectable = mgr.hasComponent(e, id);
        if (!is_selectable)
          return;

        info.m_serializeCallback(emitter, e);
    });

    emitter << GS_YAML::EndMap; // End: Entity
}

void
SceneSerializer::serialize(const std::string &filepath)
{
    GS_YAML::Emitter emitter;
    emitter << YAML::BeginMap;

    // TODO: Fix as this needs to be stored and saved.
    emitter << GS_YAML::Key << "Scene" << GS_YAML::Value << "Untitled";

    emitter << GS_YAML::Key << "Entities" << GS_YAML::Value << GS_YAML::BeginSeq;
    m_scene->m_entityManager.registry().each([&](auto eid){
       Entity e{ eid, m_scene->m_entityManager };
       if (!e)
           return;

       gsSerializeEntity(emitter, e, m_scene->m_entityManager);
    });
    emitter << GS_YAML::EndSeq;

    emitter << GS_YAML::EndMap;

    std::ofstream fstream(filepath);
    fstream << emitter.c_str();
}

bool
SceneSerializer::deserialize(const std::string &filepath)
{
    std::ifstream stream(filepath);
    std::stringstream stringstream;
    stringstream << stream.rdbuf();

    YAML::Node data = YAML::Load(stringstream.str());
    if (!data["Scene"])
        return false;

    std::string scene_name = data["Scene"].as<std::string>();

    auto entities = data["Entities"];
    if (!entities)
        return false;

    for (auto&& yaml_e : entities)
    {
        std::string name;
        auto tag = yaml_e[TagComponent::theTypename];
        if (tag)
            name = tag["name"].as<std::string>();

        Entity entity = m_scene->createEntity(name);
        // Force deserialize the transform component.
        {
            TransformComponent& transform = entity.getComponent<TransformComponent>();
            GS_YAML::Node node = yaml_e[TransformComponent::theTypename];
            transform.deserialize(node);
        }

        const auto tag_type = rttr::type::get<TagComponent>();
        const auto transform_type = rttr::type::get<TransformComponent>();

        ComponentTypeRegistry::each([&](auto , const auto& info)
        {
            if (info.m_type == tag_type || info.m_type == transform_type)
                return;

            info.m_deserializeCallback(yaml_e, entity);
        });
    }

    return true;
}

}
