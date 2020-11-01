//
// Created by matt on 2020-11-01.
//

#ifndef DOGBONE_GS_YAML_H
#define DOGBONE_GS_YAML_H

#include "GS_API.h"

#include <glm/glm.hpp>

#include <yaml-cpp/yaml.h>

namespace GS_YAML = YAML;

inline GS_YAML::Emitter& operator<<(GS_YAML::Emitter& emitter, const glm::vec3& v)
{
    emitter << GS_YAML::Flow;
    emitter << GS_YAML::BeginSeq << v.x << v.y << v.z << GS_YAML::EndSeq;
    return emitter;
}

inline GS_YAML::Emitter& operator<<(GS_YAML::Emitter& emitter, const glm::vec4& v)
{
    emitter << GS_YAML::Flow;
    emitter << GS_YAML::BeginSeq << v.x << v.y << v.z << v.w << GS_YAML::EndSeq;
    return emitter;
}

namespace YAML
{
template <>
struct convert<glm::vec3>
{
    static Node encode(const glm::vec3& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        return node;
    }
    static bool decode(const Node& node, glm::vec3& rhs)
    {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};
template <>
struct convert<glm::vec4>
{
    static Node encode(const glm::vec4& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        return node;
    }

    static bool decode(const Node& node, glm::vec4& rhs)
    {
        if (!node.IsSequence() || node.size() != 4)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};


}

#endif // DOGBONE_GS_YAML_H
